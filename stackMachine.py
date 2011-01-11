#add modes

MAXVAL = 1000000000000

class StackMachine:
    def __init__(self, maxjumps, maxbits, stackSize=10, defaultVal=1):
        self.maxbits = maxbits
        self.maxjumps = maxjumps
        self.stackSize = stackSize
        self.defaultVal = defaultVal
        self.suffix = [self._out, self._push(0), self._jmp]

        self.opcodes = []
        self.fillOps()

    def reset(self):
        self.curjumps = 0
        self.cmp = False
        self.stack = [self.defaultVal] * self.stackSize
        self.output = []
        self.stackptr = 0
        self.iptr = 0
        self.running = True
        
    def execute(self, prog):
        self.reset()
        prog.extend(self.suffix)
        while self.iptr < len(prog):
            if not prog[self.iptr]():
                self.iptr += 1
            
        return self.output

    def pop(self):
        x = self.stack[self.stackptr]
        self.stackptr = (self.stackptr - 1) % self.stackSize
        return x

    def peek(self):
        return self.stack[self.stackptr]

    def push(self, x):
        if x > MAXVAL or x < -MAXVAL:
            x %= MAXVAL
        self.stackptr = (self.stackptr + 1) % self.stackSize
        self.stack[self.stackptr] = x

    def _jmp(self):
        self.curjumps += 1
        if self.curjumps >= self.maxjumps:
            return False

        self.iptr = self.pop() % self.stackSize
        return True

    def _out(self):
        self.output.append(int(self.peek()))

    def _push(self, x):
        def f():
            self.push(x)

        return f
    
    def _swap(self):
        a, b = self.pop(), self.pop()
        self.push(b)
        self.push(a)

    def _rot(self):
        self.pop()

    def _over(self):
        pass

    def _dup(self):
        self.push(self.peek())

    def _iftrue(self):
        if not self.cmp:
            self.iptr += 1

    def _less(self):
        self.cmp = (self.pop() < self.pop())

    def _equal(self):
        self.cmp = (self.pop() == self.pop())

    def _plus(self):
        self.push(self.pop() + self.pop())

    def _neg(self):
        self.push(-self.pop())

    def _mul(self):
        self.push(self.pop() * self.pop())

    def _div(self):
        a = self.pop()
        b = self.pop()
        if b == 0:
            if a >= 0:
                self.push(MAXVAL)
            else:
                self.push(-MAXVAL)
        else:
            self.push(a//b)

    def _and(self):
        self.push(self.pop() & self.pop())

    def _or(self):
        self.push(self.pop() | self.pop())

    def _xor(self):
        self.push(self.pop() ^ self.pop())

    def fillOps(self):
        self.opcodes.append((self._rot, 5))
        self.opcodes.append((self._over, 5))
        self.opcodes.append((self._swap, 5))
        self.opcodes.append((self._div, 5))
        self.opcodes.append((self._or, 5))
        self.opcodes.append((self._and, 5))
        self.opcodes.append((self._equal, 5))
        self.opcodes.append((self._neg, 5))

        self.opcodes.append((self._plus, 4))
        self.opcodes.append((self._less, 4))
        self.opcodes.append((self._iftrue, 4))
        self.opcodes.append((self._xor, 4))
        self.opcodes.append((self._jmp, 4))
        self.opcodes.append((self._mul, 4))
        self.opcodes.append((self._out, 4))
        self.opcodes.append((self._dup, 4))

        for i in xrange(self.maxbits - 2):
            self.opcodes.append((self._push(i), 3 + i))
