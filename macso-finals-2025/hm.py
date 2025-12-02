mapping = {
    "ZERO": 0,
    "ONE": 1,
    "TWO": 2,
    "THREE": 3,
    "FOUR": 4,
    "FIVE": 5
}

nums = [-1]
# read all lines
with open('input.txt', 'r') as f:
    for line in f:
        parts = line.strip().split('\t')
        nums.append(mapping[parts[1]])

for i in range(len(nums)):
    if i%3==2:
        nums[i] -= 1
    if i%19==2:
        nums[i] -= 1
    if i%29==6:
        nums[i] -= 1
    if i%31==8:
        nums[i] -= 1
    if i%26==9:
        nums[i] -= 1

for i in range(1, len(nums)):
    if nums[i] > 0:
        print(f"{i} {nums[i]}")
    if nums[i] < 0:
        print("ERROR")