# Re-run the brute-force with "spirit, 2" replaced by "banish, 2".
from itertools import permutations
import pandas as pd

# Tiles: (value, mod)
tiles = [
    (0, 18),
    (2, 8),
    (16, 12),
    (27, 10),
    (60, 12),
    (248, 6),
    (1000, 17),
]

# Cards: (name, x)  -- spirit -> banish (same x=2)
cards = [
    ("scrap", 9),
    ("??????????", 9),
    ("consume", 3),
    ("exile", 3),
    ("banish", 2),   # replaced here
    ("exhaust", 9),
    ("trash", 2),
]

def extract_char(name: str, idx: int, mode: str):
    if mode == 'zero':
        if 0 <= idx < len(name):
            return name[idx]
        return None
    elif mode == 'one':
        if 1 <= idx <= len(name):
            return name[idx-1]
        return None

def simulate(assignment, mode: str):
    out_chars = []
    z_list = []
    for (val, mod), (name, x) in zip(tiles, assignment):
        y = val + x
        z = y % mod
        ch = extract_char(name, z, mode)
        if ch is None:
            return None
        out_chars.append(ch)
        z_list.append(z)
    return "".join(out_chars), z_list

def brute(mode: str):
    rows = []
    for perm in permutations(cards):
        res = simulate(perm, mode)
        if res is None: 
            continue
        s, zs = res
        row = {
            "result_string": s,
        }
        for i, ((tval, tmod), (cname, cx), z) in enumerate(zip(tiles, perm, zs), start=1):
            row[f"tile{i}_value"] = tval
            row[f"tile{i}_mod"] = tmod
            row[f"tile{i}_card"] = cname
            row[f"tile{i}_x"] = cx
            row[f"tile{i}_z"] = z if mode == 'zero' else f"{z} (1-indexed)"
        rows.append(row)
    return pd.DataFrame(rows)

df_zero = brute('zero')
df_one = brute('one')

# Save to files
zero_path = "valid_zero_index_banish.csv"
one_path = "valid_one_index_banish.csv"
df_zero.sort_values(["result_string"] + [f"tile{i}_card" for i in range(1,8)]).to_csv(zero_path, index=False)
df_one.sort_values(["result_string"] + [f"tile{i}_card" for i in range(1,8)]).to_csv(one_path, index=False)

print(f"Saved full CSVs:\n  - {zero_path}\n  - {one_path}")
