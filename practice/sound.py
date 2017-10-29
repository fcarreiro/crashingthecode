# http://www.boi2007.de/tasks/sound.pdf
# Solved moving window using MonotoneQueue
# Test cases found at http://www.boi2007.de/tasks/testdata-sound.tar.gz
# Execute as "python sound.py sound.i*"

import sys
from collections import deque

class MonotoneQueue:
    def __init__(self, cmp = lambda x,y: x < y):
        self.cmp = cmp
        self.q = deque()
        self.s = 0

    def front(self):
        value, count = self.q[0]
        return value

    def pop(self):
        value, count = self.q[0]
        self.q.popleft()
        if count > 1:
            self.q.appendleft((value, count - 1))
        self.s -= 1

    def push(self, value):
        count = 1
        while len(self.q) > 0 and self.cmp(self.q[-1][0], value):
            count += self.q[-1][1]
            self.q.pop()
        self.q.append((value, count))
        self.s += 1

    def size(self):
        return self.s

    def __str__(self):
        return str(self.q)

def find_silences(samples, reqlength, maxnoise):
    mins = MonotoneQueue(lambda x, y: x > y)
    maxs = MonotoneQueue(lambda x, y: x < y)
    silences = []

    for i, e in enumerate(samples + [0], 1):
        if mins.size() >= reqlength and maxs.size() >= reqlength:
            noise_diff = abs(maxs.front() - mins.front())
            mins.pop()
            maxs.pop()
            if noise_diff <= maxnoise:
                # print 'SILENCE FOUND at [', i - reqlength, ',', i, ']'
                silences.append(i - reqlength)
        mins.push(e)
        maxs.push(e)
        # print str(i) + ':', mins.front(), maxs.front() # , mins, maxs

    return silences

def main():
    for iname in sys.argv[1:]:
        f = open(iname)
        print '>', iname
        nsamples, reqlength, maxnoise = map(int, f.readline().split())
        print 'reqlength:', reqlength, 'maxnoise:', maxnoise
        samples = map(int, f.readline().split())
        f.close()

        silences = find_silences(samples, reqlength, maxnoise)
        # print 'Found silences at:', silences

        outname = iname.replace('i', 'o')
        fout = open(outname)
        expected = [int(num) for num in fout.read().split() if num != 'NONE']
        fout.close()
        # print 'Expected silences at:', expected
        print 'Found == Expected?', expected == silences
        if expected != silences:
            print 'found', silences
            print 'expected', expected

main()
