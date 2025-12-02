# A = 1, Z = 26, AA = 27, AAA = 703

import re

def encode(s):
    result = 0
    for i, char in enumerate(reversed(s)):
        result += (ord(char) - ord('A') + 1) * (26 ** i)
    return result

def decode(n):
    result = []
    while n > 0:
        n -= 1
        result.append(chr(n % 26 + ord('A')))
        n //= 26
    return ''.join(reversed(result))

a = input().strip()[1:-2].split(', ')
a = [int(x) for x in a]
print(a)
for i, x in enumerate(a):
    if abs(x) == 4534:
        print(f"Found {x} at index {i+1}")

# with open('input.txt', 'r') as f:
#     for line in f:
#         x = int(line.strip())
#         print(decode(x))