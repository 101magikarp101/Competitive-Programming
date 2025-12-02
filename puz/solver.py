#!/usr/bin/env python3
"""
Masyu brute-force solver with manual input + dictionary filter for final words,
and extraction of letters strictly inside the loop (not on the loop or outside).

USAGE
-----
1) Put your puzzle in a text file in one of the formats below, then run:
   python3 masyu_solver_manual_dict.py path/to/input.txt

2) If you run with no arguments, it uses the SAMPLE at the bottom of this file
   (the 10x10 puzzle from the prompt).

SUPPORTED INPUT FORMATS
-----------------------
A) GRID FORMAT (case-insensitive):
   - First non-empty line: either "N=10" or just "10" for a 10x10 grid.
   - Somewhere a line "GRID:" followed by N lines of length N using:
       . or _  = empty
       W       = white pearl
       B       = black pearl
   - Optional "LETTERS:" followed by N lines of length N (A..Z) to extract a
     row-major string from cells strictly inside the loop (and not on it).

Example:
--------
N=10
GRID:
.W.WW...W.
.....B.B..
B.........
..W......W
..W.....W.
.....B....
W.........
..........
.......W.W
B.........

LETTERS:
YOGASTUDIO
TRANSCRIBE
CONSTRAINT
RACECOURSE
REVOLUTION
REDHERRING
INTERWOVEN
PENCHANTTM
PUZZLEHUNT
DRAWBRIDGE

B) JSON FORMAT:
{
  "N": 10,
  "white": [[1,0],[1,2],[1,3], ...],
  "black": [[2,5],[2,7], ...],
  "letters": ["YOGASTUDIO","TRANSCRIBE", ...]  // optional
}

DICTIONARY FILTER
-----------------
--dict PATH   Path to a newline-delimited lowercase word list (default: words.txt).
              Only strings that appear exactly in this file are printed.
              If the file is missing/unreadable, the script prints all strings.

Notes
-----
- Coordinates are 0-indexed (row, col).
- Time limit and maximum number of solutions can be tuned via CLI flags:
    --time 120   (seconds)
    --max  5000  (solutions)
"""

import sys, math, json, argparse, time, os, re
from collections import defaultdict, Counter
from copy import deepcopy

# --- Parsing ---

def parse_grid_format(text):
    lines = [ln.strip() for ln in text.splitlines() if ln.strip() and not ln.strip().startswith('#')]
    if not lines:
        raise ValueError("Empty input.")
    first = lines.pop(0)
    if first.upper().startswith("N="):
        N = int(first.split("=",1)[1])
    else:
        N = int(first)
    white=set(); black=set()
    letters=None
    try:
        gidx = next(i for i,ln in enumerate(lines) if ln.upper().startswith("GRID"))
    except StopIteration:
        raise ValueError("GRID: section not found.")
    grid_lines = lines[gidx+1:gidx+1+N]
    if len(grid_lines)!=N:
        raise ValueError(f"GRID section must have {N} lines.")
    for r,ln in enumerate(grid_lines):
        ln = ln.strip().replace(" ","")
        if len(ln)!=N:
            raise ValueError(f"GRID row {r} has length {len(ln)} but expected {N}.")
        for c,ch in enumerate(ln.upper()):
            if ch=='W':
                white.add((r,c))
            elif ch=='B':
                black.add((r,c))
            elif ch in ('.','_'):
                pass
            else:
                raise ValueError(f"Bad GRID char '{ch}' at ({r},{c}). Use '.', 'W', or 'B'.")
    # optional LETTERS:
    try:
        lidx = next(i for i,ln in enumerate(lines) if ln.upper().startswith("LETTERS"))
        letter_lines = lines[lidx+1:lidx+1+N]
        if len(letter_lines)!=N:
            raise ValueError(f"LETTERS section must have {N} lines.")
        letters = [ln.strip().replace(" ","").upper() for ln in letter_lines]
        for i,row in enumerate(letters):
            if len(row)!=N or not row.isalpha():
                raise ValueError(f"LETTERS row {i} must be {N} letters A-Z.")
    except StopIteration:
        letters=None
    return N, white, black, letters

def parse_json_format(text):
    obj = json.loads(text)
    N = int(obj["N"])
    white = set(map(tuple, obj["white"]))
    black = set(map(tuple, obj["black"]))
    letters = obj.get("letters", None)
    if letters is not None:
        if len(letters)!=N or any(len(row)!=N for row in letters):
            raise ValueError("letters must be N strings of length N.")
        letters = [row.upper() for row in letters]
    return N, white, black, letters

def load_manual_input(path=None):
    if path is None:
        return SAMPLE()
    text = open(path,"r",encoding="utf-8").read()
    try:
        return parse_json_format(text)
    except Exception:
        return parse_grid_format(text)

# --- Solver ---

# bit patterns [N,E,S,W] bits at [3,2,1,0]
EMPTY=0
VERT=0b1010; HORZ=0b0101
NE=0b1100; SE=0b0110; SW=0b0011; NW=0b1001

ALL = [EMPTY, VERT, HORZ, NE, SE, SW, NW]
STRAIGHTS = [VERT, HORZ]
TURNS = [NE,SE,SW,NW]

# (dr,dc,opposite)
DIRS = [(-1,0,2), (0,1,3), (1,0,0), (0,-1,1)]

def edge_val(mask, dir_index):
    return (mask >> (3-dir_index)) & 1

def valid_for_cell(N, r,c,mask):
    n,e,s,w = ((mask>>3)&1,(mask>>2)&1,(mask>>1)&1,mask&1)
    if r==0 and n: return False
    if r==N-1 and s: return False
    if c==0 and w: return False
    if c==N-1 and e: return False
    return True

def restrict_by_neighbor(dom, required_edge, dir_index):
    return {m for m in dom if edge_val(m,dir_index)==required_edge}

def propagate(N, white, black, assign, doms):
    changed=True
    while changed:
        changed=False
        # adjacency
        for (r,c),mask in list(assign.items()):
            for i,(dr,dc,oppo) in enumerate(DIRS):
                nr,nc=r+dr,c+dc
                val = edge_val(mask,i)
                if 0<=nr<N and 0<=nc<N:
                    newdom = restrict_by_neighbor(doms[nr][nc], val, oppo)
                    if newdom != doms[nr][nc]:
                        doms[nr][nc] = newdom
                        if not newdom:
                            return False
                        if len(newdom)==1 and (nr,nc) not in assign:
                            assign[(nr,nc)] = next(iter(newdom)); changed=True
                else:
                    if val==1: return False
        # black adjacency "straight before/after"
        for (r,c),mask in list(assign.items()):
            if (r,c) in black:
                dirs_used = [i for i in range(4) if edge_val(mask,i)==1]
                for i in dirs_used:
                    dr,dc,oppo = DIRS[i]
                    nr,nc=r+dr,c+dc
                    req = VERT if i in (0,2) else HORZ
                    if 0<=nr<N and 0<=nc<N:
                        old = doms[nr][nc]
                        new = {req} if req in old else set()
                        if new != old:
                            doms[nr][nc] = new
                            if not new: return False
                            if (nr,nc) not in assign:
                                assign[(nr,nc)] = req; changed=True
                    else:
                        return False
    return True

def finalize_singletons(N, assign, doms, white, black):
    changed=True
    while changed:
        changed=False
        for r in range(N):
            for c in range(N):
                if (r,c) not in assign and len(doms[r][c])==1:
                    assign[(r,c)] = next(iter(doms[r][c]))
                    if not propagate(N, white, black, assign, doms):
                        return False
                    changed=True
    return True

def detect_premature_cycle(N, forced_cells, assign):
    edges = defaultdict(set)
    for (r,c),m in assign.items():
        if m==EMPTY: continue
        for i,(dr,dc,oppo) in enumerate(DIRS):
            if edge_val(m,i)==1:
                nr,nc=r+dr,c+dc
                if (nr,nc) in assign and edge_val(assign[(nr,nc)],oppo)==1:
                    edges[(r,c)].add((nr,nc))
    visited=set(); cycles=[]
    for node in edges:
        if node in visited: continue
        comp=set(); q=[node]
        while q:
            u=q.pop()
            if u in visited: continue
            visited.add(u); comp.add(u)
            for v in edges[u]:
                if v not in visited: q.append(v)
        if all(len(edges[u])==2 for u in comp):
            cycles.append(comp)
    if not cycles: return False
    union=set().union(*cycles)
    for fc in forced_cells:
        if fc not in union: return True
    if len(cycles)>1: return True
    return False

def check_white_adjacent(N, white, assign):
    for (r,c) in white:
        m = assign[(r,c)]
        if m==VERT:
            if r-1<0 or r+1>=N: return False
            top = assign[(r-1,c)]; bot = assign[(r+1,c)]
            if edge_val(top,2)!=1 or edge_val(bot,0)!=1: return False
            if not ((top in {SE,SW}) or (bot in {NE,NW})): return False
        elif m==HORZ:
            if c-1<0 or c+1>=N: return False
            left = assign[(r,c-1)]; right = assign[(r,c+1)]
            if edge_val(left,1)!=1 or edge_val(right,3)!=1: return False
            if not ((left in {NE,SE}) or (right in {NW,SW})): return False
        else:
            return False
    return True

def is_single_loop(N, assign):
    parent={}
    def find(x):
        parent.setdefault(x,x)
        if parent[x]!=x: parent[x]=find(parent[x])
        return parent[x]
    def union(a,b):
        ra, rb = find(a), find(b)
        if ra!=rb: parent[rb]=ra
    used=set(); degree=defaultdict(int)
    for r in range(N):
        for c in range(N):
            m = assign[(r,c)]
            if m==EMPTY: continue
            used.add((r,c))
            for i,(dr,dc,oppo) in enumerate(DIRS):
                if edge_val(m,i)==1:
                    nr,nc=r+dr,c+dc
                    if 0<=nr<N and 0<=nc<N and edge_val(assign[(nr,nc)],oppo)==1:
                        union((r,c),(nr,nc)); degree[(r,c)]+=1
                    else:
                        return False
    if not used: return False
    if any(degree[u]!=2 for u in used): return False
    roots={find(u) for u in used}
    return len(roots)==1

def choose_var(N, assign, doms):
    best=None; size=1e9
    for r in range(N):
        for c in range(N):
            if (r,c) not in assign:
                d=len(doms[r][c])
                if d==0: return None
                if d>1 and d < size:
                    size=d; best=(r,c)
    return best

def solve_masyu(N, white, black, time_limit=120, max_solutions=5000):
    domains = [[{m for m in (EMPTY,VERT,HORZ,NE,SE,SW,NW) if valid_for_cell(N,r,c,m)} for c in range(N)] for r in range(N)]
    for r,c in white: domains[r][c] &= {VERT, HORZ}
    for r,c in black: domains[r][c] &= {NE,SE,SW,NW}
    for r in range(N):
        for c in range(N):
            if (r,c) in white or (r,c) in black:
                domains[r][c] -= {EMPTY}

    forced_cells = set(white)|set(black)
    solutions=[]
    start=time.time()

    def backtrack(assign, doms):
        if time.time()-start > time_limit: return
        doms=deepcopy(doms); assign=dict(assign)
        if not propagate(N, white, black, assign, doms): return
        if not finalize_singletons(N, assign, doms, white, black): return
        if detect_premature_cycle(N, forced_cells, assign): return
        if all((r,c) in assign for r in range(N) for c in range(N)):
            if check_white_adjacent(N, white, assign) and is_single_loop(N, assign):
                solutions.append(assign)
            return
        var = choose_var(N, assign, doms)
        if var is None:
            for r in range(N):
                for c in range(N):
                    if (r,c) not in assign:
                        var=(r,c); break
                if var: break
        r,c = var
        vals = sorted(doms[r][c], key=lambda m: (m==EMPTY, m))
        for v in vals:
            if len(solutions)>=max_solutions: break
            assign[(r,c)] = v
            backtrack(assign, doms)
            del assign[(r,c)]

    backtrack({}, domains)
    return solutions

# --- Extraction (strictly inside the loop) & dictionary filter ---

def extract_string(N, letters, assign):
    """
    Return letters from cells strictly inside the single loop (Jordan interior),
    excluding any cells that are part of the loop. Uses vertical-ray casting
    from each cell center and counts intersections with the loop's horizontal
    segments. Even/odd rule.
    """
    # collect horizontal edges of the loop (E edges only to avoid duplicates)
    horiz = []  # each item: (y, x0, x1) with x0 < x1
    for r in range(N):
        for c in range(N):
            m = assign[(r,c)]
            # E edge present?
            if (m >> 2) & 1:  # dir_index=1 => bit position 2
                y = r + 0.5
                x0 = c + 0.5
                x1 = c + 1.5
                horiz.append((y, x0, x1))

    def inside_cell(r, c):
        # vertical ray upward from (x, y) = (c+0.5+eps, r+0.5+eps)
        x = c + 0.5 + 1e-7  # epsilon to avoid landing exactly on vertices
        y = r + 0.5 + 1e-7
        crossings = 0
        for (yh, x0, x1) in horiz:
            if yh <= y:  # only consider edges strictly above the point
                continue
            if x0 <= x < x1:
                crossings += 1
        return (crossings % 2) == 1

    out = []
    for r in range(N):
        for c in range(N):
            if assign[(r,c)] == EMPTY:
                if inside_cell(r, c):
                    out.append(letters[r][c])
    return "".join(out)

def extract_string2(N, letters, assign):
    """
    Original method:
    Return letters from all cells that are NOT used by the loop,
    without checking inside/outside.
    """
    out = []
    for r in range(N):
        for c in range(N):
            if assign[(r,c)] == EMPTY:
                out.append(letters[r][c])
    return "".join(out)

def load_dict(path):
    try:
        with open(path,"r",encoding="utf-8") as f:
            words = {ln.strip() for ln in f if ln.strip()}
        # keep only lowercase alpha
        words = {w for w in words if w.isalpha() and w == w.lower()}
        return words
    except Exception as e:
        print(f"[warn] Could not read dictionary '{path}': {e}")
        return None

# --- Unique word frequency reporting ---

def unique_word_frequencies(words):
    """
    Return a list of (word, freq) sorted alphabetically by word.
    Words are normalized to lowercase for counting.
    """
    c = Counter(w.lower() for w in words)
    return sorted(c.items(), key=lambda kv: kv[0])

def report_unique_words(words, heading="\nUnique words (alpha) with frequencies:"):
    """
    Print unique words and their frequencies in alphabetical order.
    """
    items = unique_word_frequencies(words)
    print(heading)
    if not items:
        print("(none)")
        return
    for w, f in items:
        print(f"{w} {f}")

# --- Sample (matches your prompt) ---
def SAMPLE():
    N=10
    white={(1,0),(1,2),(1,3),(2,8),(4,2),(4,9),(6,0),(7,1),(8,7),(8,9)}
    black={(2,5),(2,7),(3,0),(5,6),(6,9),(9,0)}
    letters=[
        "YOGASTUDIO",
        "TRANSCRIBE",
        "CONSTRAINT",
        "RACECOURSE",
        "REVOLUTION",
        "REDHERRING",
        "INTERWOVEN",
        "PENCHANTTM",
        "PUZZLEHUNT",
        "DRAWBRIDGE",
    ]
    return N, white, black, letters

# --- CLI ---
def main():
    ap = argparse.ArgumentParser(description="Masyu solver with manual input, inside-only extraction, and dictionary filtering.")
    ap.add_argument("path", nargs="?", help="Path to input file (grid or JSON). If omitted, uses built-in sample.")
    ap.add_argument("--time", type=int, default=120, help="Time limit in seconds (default: 120)")
    ap.add_argument("--max", type=int, default=5000, help="Max solutions to collect (default: 5000)")
    ap.add_argument("--dict", dest="dict_path", default="words.txt", help="Path to newline-delimited lowercase word list (default: words.txt)")
    args = ap.parse_args()

    N, white, black, letters = load_manual_input(args.path)
    print(f"Loaded N={N}, whites={len(white)}, blacks={len(black)}, letters={'yes' if letters else 'no'}")

    sols = solve_masyu(N, white, black, time_limit=args.time, max_solutions=args.max)
    print(f"Found {len(sols)} solution(s).")

     # Load dictionary (optional but recommended)
    dict_words = load_dict(args.dict_path)

    # --- Choose which extractor to use ---
    # Default: inside-only (strictly inside the loop)
    extractor = extract_string
    # If you prefer the original behavior (every unused cell), switch to:
    # extractor = extract_string2

    # Keep only boards that have a valid word (if dictionary is available).
    kept = []   # list of (solution, word)
    kept_words = []

    for sol in sols:
        w = extractor(N, letters, sol) if letters else ""
        if dict_words is not None:
            if w and (w.lower() in dict_words):
                kept.append((sol, w))
                kept_words.append(w)
        else:
            # No dictionary available: treat all as "valid"
            kept.append((sol, w))
            if w:
                kept_words.append(w)

    # Report results
    if dict_words is not None:
        print(f"\nBoards with valid words in '{args.dict_path}': {len(kept)}")
    else:
        print(f"\nBoards (no dictionary filter): {len(kept)}")

    sym={EMPTY:'.', VERT:'|', HORZ:'-', NE:'└', SE:'┌', SW:'┐', NW:'┘'}

    if not kept:
        print("(none)")
    else:
        for i,(sol,w) in enumerate(kept, 1):
            if letters:
                print(f"[{i}] {w}")
            # Print the board for this solution
            print(f"\nSolution {i} map (.'=unused, | - and corners):")
            for r in range(N):
                print("".join(sym[sol[(r,c)]] for c in range(N)))

        # Also print unique-word frequencies (alpha order) for the kept set
        if letters:
            report_unique_words(kept_words)

if __name__ == "__main__":
    main()
