import sys
import math
import collections
import random
from functools import cache
from heapq import heappush, heappop

input = sys.stdin.readline
 
ints = lambda: list(map(int, input().split()))

n = int(input())
a = ints()
best = -1
mn = math.inf
for i in range(n):
    sp = 1
    tot = 0
    for j in range(i - 1, -1, -1):
        dі = abs(a[j] - a[j + 1])
        tot += dі/sp
        sp += dі
    tot += a[0]/sp
    if tot < mn:
        mn = tot
        best = i
    #print(i, tot)
print(best+1)