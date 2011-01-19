import PyMachine

def mapSeq(fname, maxi=1000000000):
    d = {}
    f = open(fname)
    i = 0
    for l in f:
        i += 1
        if i % 100000 == 0:
            print i
        if i >= maxi:
            break
        nr, depth, program, seq = l.split(' ', 3)
        d[seq.strip()] = (nr,depth,program)
    f.close()
    return d

def mapOEIS(fname='stripped'):
    d = {}
    f = open(fname)
    for l in f:
        l.strip()
        if l.startswith('#'):
            continue

        seqs = l.split(',')
        s = ' '.join(seqs[1:11])
        if s not in d:
            d[s] = []
        d[s].append(seqs[0].strip())

    f.close()
    return d

def checkSeq(fname, moe):
    dpth = [0]*40
    f = open(fname)
    
    cnt = 0
    for l in f:
        nr, depth, program, seq = l.split(' ', 3)
        seq = seq.strip()
        if seq in moe:
            #print seq, ':', nr,depth,program, moe[seq]
            dpth[int(depth)] += 1
            cnt += 1

    f.close()
    print cnt
    return dpth

def findPrimes(fname, allowedMiss=0):
    ps = []
    f = open(fname)
    i = 0
    j = 0
    for l in f:
        i += 1
        nr, depth, program, seq = l.split(' ', 3)
        seq = seq.strip()

        xs = [int(x) for x in seq.split()]

        if len(set(xs)) != 10: continue

        good = True
        for x in xs:
            if x < 1:
                good = False
                break

        if not good: continue

        j += 1
        misses = 0
        for x in xs:
            if not isPrime(x):
                misses += 1
                if misses > allowedMiss:
                    break

        if misses <= allowedMiss:
            print misses, xs, nr, depth, program
            ps.append((program, depth))
    print i, j
            
    f.close()
    return ps

def isPrime(n):
    '''check if integer n is a prime'''
    if n == 1:
        return True
    if n == 2: 
        return True    

    if not n & 1: 
        return False

    # range starts with 3 and only needs to go up the squareroot of n
    # for all odd numbers
    for x in xrange(3, int(n**0.5)+1, 2):
        if n % x == 0:
            return False
    return True

def extendPotentialPrimes(fname, maxIterations, allowedMiss):
    ps = findPrimes(fname, allowedMiss)
    pm = PyMachine.PyMachine(maxIterations, 1, maxIterations, 0, maxIterations)
    
    extended = []
    for program, plen in ps:
        pm.load(long(program), int(plen))
        pm.execute()
        oldp = -1
        outputs = []
        nrisp = 0
        v = pm.getOutput()
        print [x for x in v]
        for p in v:
            if p <= oldp:
                break

            isp = isPrime(p)
            nrisp += isp
            outputs.append((p, isp))

        if nrisp:
            extended.append((float(nrisp) / len(outputs), nrisp, isp, outputs, program, plen))
    extended.sort()
    extended.reverse()

    return extended
