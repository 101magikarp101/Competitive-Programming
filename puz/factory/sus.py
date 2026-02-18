x = int(input())

primes = []
M = 25
is_prime = [False] * M
# linear sieve
for i in range(2, M):
    if not is_prime[i]:
        primes.append(i)
    for p in primes:
        if i * p >= M:
            break
        is_prime[i * p] = True
        if i % p == 0:
            break

for p in primes:
    cnt = 0
    while x % p == 0:
        x //= p
        cnt += 1
    if cnt > 0:
        print(p, cnt)

print(x)