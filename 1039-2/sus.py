def collatz(n):
    if n == 1:
        print(n)
        return
    elif n % 2 == 0:
        print(n)
        collatz(n // 2)
    else:
        print(n)
        collatz(3 * n + 1)

if __name__ == "__main__":
    collatz(4849)