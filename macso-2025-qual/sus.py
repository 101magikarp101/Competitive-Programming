def digit_sum(x: int) -> int:
    s = 0
    while x:
        s += x % 10
        x //= 10
    return s

MAX_N = 1_000_000    # stop after this many tries

results = {}
for k in range(2, 10, 2):
    found = False
    for n in range(1, MAX_N+1):
        if digit_sum(k*n) * k == digit_sum(n):
            results[k] = n
            found = True
            break
    if not found:
        results[k] = -1

# Print out:
for k in range(2, 101, 2):
    print(f"k = {k:3d} → n = {results[k]}")
