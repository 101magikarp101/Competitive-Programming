# Brute-force the smallest 1-indexed position <= 10_000 where each configuration
# column (1..6) matches ALL rows' repeating sequences.
#
# Inputs:
#  - TARGETS: your corrected table (each inner list = a row, columns 1..6)
#  - ROTATION_SPECS: the repeating sequences from your original "rotation" column
#    (axb means "a" repeated b times). Order matches TARGETS row order.
#
# No CRT used; just straight brute force.

from typing import List

def expand_rotation(spec: str) -> List[int]:
    """Expand a rotation spec like '29x10 14 29x6 ...' into a list of ints."""
    seq: List[int] = []
    for tok in spec.split():
        if "x" in tok:
            val_str, cnt_str = tok.split("x", 1)
            val = int(val_str)
            cnt = int(cnt_str)
            seq.extend([val] * cnt)
        else:
            seq.append(int(tok))
    return seq

# ---- Your corrected per-row targets (header row is configuration #) ----
TARGETS = [
    [5, 5, 7, 7, 7, 7],   # row 1
    [7, 7, 7, 7, 7, 7],   # row 2
    [4, 4, 4, 4, 4, 4],   # row 3
    [10,10,10,10,10,10],  # row 4
    [10,10, 8, 8, 8,10],  # row 5
    [ 7, 8, 6, 7, 7, 6],  # row 6
    [ 8, 8, 8, 8, 9, 8],  # row 7
    [ 9, 9, 9, 9, 9, 9],  # row 8
    [ 7, 7, 7, 7, 7, 7],  # row 9
    [ 6, 5, 7, 6, 5, 5],  # row 10
    [11,11,11,11,11,11],  # row 11
    [13,13,13,13,13,13],  # row 12
    [ 8, 8, 8, 8, 8, 8],  # row 13
    [14,14,14,14,14,14],  # row 14
    [ 7, 7, 7, 7, 7, 7],  # row 15
]

# ---- Repeating sequences from your original "rotation" column (row-aligned) ----
ROTATION_SPECS = [
    "7 5",  # row 1  (5/7)
    "7",    # row 2  (7)
    "4",    # row 3  (4)
    "10",   # row 4  (10)
    "10 8 10 8 7 7 7",  # row 5  (7/8/10)
    "8 7 6",               # row 6  (6/7/8)
    "11 8 8 11 8 11 9 8 11 11 11",  # row 7  (8/9/11)
    "9",    # row 8  (9)
    "7",    # row 9  (7)
    "5 6 5 5 7",  # row 10 (5/6/7)
    "13 11 13 13 11 13 13 11 13 13 11 11 13",  # row 11 (11/13)
    "17 13 13 13 17 17 17 17 17 13 17 13 17 17 17 17 17",  # row 12 (13/17)
    "19 19 19 19 19 19 8 19 8 19 19 8 8 19 19 19 8 19 19",  # row 13 (8/19)
    "29x10 14 29x6 14 29x6 14 14 29x3",  # row 14 (14/29)
    "23 7x2 23x8 7 23 7 23x7 7 23",      # row 15 (7/23)
]

assert len(TARGETS) == len(ROTATION_SPECS), "Rows of TARGETS and ROTATION_SPECS must match"

# Expand rotation specs to concrete lists
ROTATIONS: List[List[int]] = [expand_rotation(s) for s in ROTATION_SPECS]
print("Expanded rotation sequences:")
for r in ROTATIONS:
    print(r, len(r))

MAX_N = 10_000
NUM_COLS = 6

# Brute force: for each configuration column, find the smallest n s.t.
# for every row r: ROTATIONS[r][(n-1) % len(ROTATIONS[r])] == TARGETS[r][c]
answers = [None] * NUM_COLS
remaining = set(range(NUM_COLS))

for n in range(1, MAX_N + 1):
    if not remaining:
        break
    for c in list(remaining):
        ok = True
        for r in range(len(TARGETS)):
            seq = ROTATIONS[r]
            want = TARGETS[r][c]
            got = seq[(n - 1) % len(seq)]
            if got != want:
                # print(f"n={n} fails row {r+1} (got {got}, want {want}) for config {c+1}")
                ok = False
                break
        if ok:
            # answers[c] = n
            print(f"Found n={n} for configuration {c+1}")
            # remaining.remove(c)

# Report
print("Smallest 1-indexed positions (<= 10000) for each configuration column:")
for c in range(NUM_COLS):
    print(f"Config {c+1}: {answers[c]}")

if remaining:
    print("\nNo solution found up to 10000 for configurations:",
          ", ".join(str(c+1) for c in sorted(remaining)))
