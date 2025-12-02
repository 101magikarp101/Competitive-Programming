#!/usr/bin/env python3
import sys
from pathlib import Path

def fail(msg: str, code: int = 1):
    print(msg.strip() + "\n", file=sys.stdout)
    sys.exit(code)

def read_ints_from_string(s: str):
    toks = s.split()
    ints = []
    for t in toks:
        try:
            ints.append(int(t))
        except ValueError:
            fail(f"Non-integer token in output: {t!r}")
    return ints

def is_ans_sentinel_minus_one(ans_path: Path) -> bool:
    if not ans_path or not ans_path.exists():
        return False
    toks = ans_path.read_text(encoding="utf-8", errors="replace").split()
    return len(toks) == 1 and toks[0] == "-1"

def main():
    # Usage: python3 check_affection.py <input_file> [answer_file]
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        fail("Usage: python3 check_affection.py <input_file> [answer_file]", 2)

    inp_path = Path(sys.argv[1])
    if not inp_path.exists():
        fail(f"Input file not found: {inp_path}", 2)

    # Optional explicit .ans path; otherwise infer <input>.ans
    ans_path = Path(sys.argv[2]) if len(sys.argv) == 3 else inp_path.with_suffix(".ans")

    # Read candidate output (stdin) once up front
    candidate_raw = sys.stdin.read()
    candidate_tokens = candidate_raw.split()

    # --- Early exit if answer is just -1 (sentinel mode) ---
    if is_ans_sentinel_minus_one(ans_path):
        if len(candidate_tokens) == 1 and candidate_tokens[0] == "-1":
            print("OK (sentinel -1)")
            sys.exit(0)
        else:
            got_preview = " ".join(candidate_tokens[:5]) if candidate_tokens else "(empty)"
            fail(f"Expected sentinel -1 per answer file, but contestant output was: {got_preview!r}")
        # (No further checks when sentinel is present.)

    # --- parse input file (N, M, then M undirected edges) ---
    raw = inp_path.read_text(encoding="utf-8", errors="replace").split()
    if len(raw) < 2:
        fail("Input file too short: expected N and M.")
    try:
        N = int(raw[0]); M = int(raw[1])
    except ValueError:
        fail("N and M must be integers.")
    need = 2 + 2*M
    if len(raw) < need:
        fail(f"Input file too short: expected {M} edges (need {need} ints, have {len(raw)}).")

    edges = set()
    idx = 2
    for _ in range(M):
        try:
            u = int(raw[idx]); v = int(raw[idx+1])
        except ValueError:
            fail("Edge endpoints must be integers.")
        idx += 2
        if not (1 <= u <= N and 1 <= v <= N):
            fail(f"Edge ({u},{v}) out of range 1..{N}.")
        if u == v:
            # Disallow self-edges by default; allow if your problem does.
            fail(f"Self-edge ({u},{v}) not allowed.")
        a, b = (u-1, v-1)
        if a > b: a, b = b, a
        edges.add((a,b))

    # --- candidate must contain exactly N*N integers (adjacency matrix weights) ---
    vals = []
    try:
        vals = [int(t) for t in candidate_tokens]
    except ValueError:
        # More precise messaging if there was a non-integer token
        for t in candidate_tokens:
            try:
                int(t)
            except ValueError:
                fail(f"Non-integer token in output: {t!r}")
        # fallback
        fail("Output parsing error.")

    if len(vals) != N*N:
        fail(f"Expected exactly {N*N} integers in output grid, got {len(vals)}.")

    grid = [vals[i*N:(i+1)*N] for i in range(N)]

    # range + row sum 10000
    for r in range(N):
        row = grid[r]
        for x in row:
            if x < 0 or x > 10000:
                fail(f"Value out of range [0,10000] at row {r+1}: {x}")
        srow = sum(row)
        if srow != 10000:
            fail(f"Row {r+1} sum is {srow}, expected 10000.")

    # symmetry
    for i in range(N):
        for j in range(N):
            if grid[i][j] != grid[j][i]:
                fail(f"Matrix not symmetric at ({i+1},{j+1}) vs ({j+1},{i+1}).")

    # non-zero only where an (undirected) edge exists
    def has_edge(i, j):
        if i == j:
            return False  # adjust if self-edges allowed
        a, b = (i, j) if i < j else (j, i)
        return (a, b) in edges

    for i in range(N):
        for j in range(N):
            if grid[i][j] != 0 and not has_edge(i, j):
                fail(f"Non-zero at ({i+1},{j+1}) but no edge in input.")

    print("OK")
    sys.exit(0)

if __name__ == "__main__":
    main()
