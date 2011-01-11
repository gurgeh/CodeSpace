import time

class Hardcopy:
    def __init__(self, machine):
        self.machine = machine
        self.opcodes = [None] * 100

    def search(self, maxdepth):
        self.nrops = 0
        self.prognr = 0
        self.store = {}
        for i in xrange(maxdepth + 1):
            a = time.time()
            print 'Checking depth', i, self.prognr
            self.bitdepth = i
            #for metaargs in opmeta
            self.searchFix(0)
            print time.time() - a

    def searchFix(self, curdepth):
        if curdepth == self.bitdepth:
            seq = tuple(self.machine.execute(self.opcodes[:self.nrops]))
            if seq not in self.store:
                self.store[seq] = []
            self.store[seq].append(self.prognr)
            self.prognr += 1
            return
        elif curdepth > self.bitdepth:
            return

        for opcode, cost in self.machine.opcodes:
            self.opcodes[self.nrops] = opcode
            self.nrops += 1
            self.searchFix(curdepth + cost)
            self.nrops -= 1

        
