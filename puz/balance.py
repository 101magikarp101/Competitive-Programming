"""
Balance Loop solver (manual input).

STANDARD RULES
--------------
Draw a single, non-intersecting loop through cell centers. The loop must pass
through every circle cell (others may be used or not). At each circle the loop
may either go straight through or turn.
- White circle: the lengths of the (straight) loop segments on the two sides
  of the circle are EQUAL.
- Black circle: those two lengths are UNEQUAL.
- If a number is given with a circle, it equals the SUM of the two lengths.

(Lengths are counted as the number of edges along each straight leg until the
next turn; the minimum length on each side is 1.)

References: GMPuzzles rules and examples; puzz.link implementation.


INPUT FORMATS
-------------

A) GRID format (case-insensitive)
    N=10
    CIRCLES:
    .W...B....
    .........W
    ..........
    ...B......
    ..........
    ..........
    W.........
    ..........
    .....B....
    ..........

    # Optional: per-cell numbers for some circles (others should be '.' )
    NUMBERS:
    ..........
    .........4
    ..........
    ...6......
    ..........
    ..........
    .8........
    ..........
    .....3....
    ..........

    # Optional: letters for extracting strings from solutions
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

- In CIRCLES, use 'W' for white, 'B' for black, '.' for empty.
- In NUMBERS, put digits (0-9) or '.'; multi-digit numbers are supported if
  you separate tokens with spaces (e.g., " . . 12 . . ").  Unnumbered circles
  should have '.' in NUMBERS. Numbers on non-circle cells are ignored.

B) JSON format
{
  "N": 10,
  "circles": [
     {"r":0,"c":1,"color":"white","sum":null},
     {"r":0,"c":5,"color":"black","sum":null},
     {"r":1,"c":9,"color":"white","sum":4},
     {"r":3,"c":3,"color":"black","sum":6}
  ],
  "letters": [
    "YOGASTUDIO","TRANSCRIBE","CONSTRAINT","RACECOURSE","REVOLUTION",
    "REDHERRING","INTERWOVEN","PENCHANTTM","PUZZLEHUNT","DRAWBRIDGE"
  ]
}


CLI
---
python3 balance_loop_solver.py puzzle.txt --time 120 --max 5000 \
  --dict words.txt --extract inside --only-valid

  --dict words.txt   Filter to only boards whose extracted string is in the
                     given lowercase word list (one word per line).
  --extract inside   Extract letters strictly inside the loop (Jordan interior).
                     Use --extract unused for the original "all non-loop cells".
  --only-valid       Only print boards that have a valid dictionary word.
                     (If --dict is missing, prints all boards.)

The solver shows ASCII maps of each kept solution.
"""

import sys, json, argparse, time, re
from collections import defaultdict, Counter
from copy import deepcopy

# ---------- Utility / model ----------

# Loop piece bitmasks [N,E,S,W] at [3,2,1,0]
EMPTY=0
VERT=0b1010; HORZ=0b0101
NE=0b1100; SE=0b0110; SW=0b0011; NW=0b1001
LOOP_PARTS = [VERT,HORZ,NE,SE,SW,NW]

DIRS = [(-1,0,2), (0,1,3), (1,0,0), (0,-1,1)]  # (dr,dc, opposite)

def edge(mask, dir_index):
    return (mask >> (3-dir_index)) & 1

def valid_for_cell(N, r, c, m):
    if m==EMPTY: return True
    n,e,s,w = edge(m,0),edge(m,1),edge(m,2),edge(m,3)
    if r==0 and n: return False
    if r==N-1 and s: return False
    if c==0 and w: return False
    if c==N-1 and e: return False
    return True

# ---------- Parsing ----------

def parse_grid_format(text):
    # keep raw lines (for multi-digit NUMBERS tokenization)
    lines = [ln.rstrip("\n") for ln in text.splitlines()]
    nz = [ln for ln in lines if ln.strip() and not ln.strip().startswith("#")]
    if not nz: raise ValueError("Empty input.")
    first = nz.pop(0).strip()
    if first.upper().startswith("N="):
        N = int(first.split("=",1)[1])
    else:
        N = int(first)

    def read_block(tag):
        try:
            idx = next(i for i,ln in enumerate(nz) if ln.strip().upper().startswith(tag))
        except StopIteration:
            return None, None
        start = idx+1
        raw = nz[start:start+N]
        if len(raw)!=N: raise ValueError(f"{tag} must have {N} lines.")
        return idx, raw

    # CIRCLES
    _, C = read_block("CIRCLES")
    if C is None: raise ValueError("Missing CIRCLES: block.")
    C = [ln.strip().replace(" ","") for ln in C]
    if any(len(r)!=N for r in C):
        raise ValueError("Each CIRCLES row must have N characters.")
    circles = {}  # (r,c) -> ('white'|'black', sum or None)
    for r in range(N):
        for c,ch in enumerate(C[r]):
            if ch in ".":
                continue
            elif ch.upper()=="W":
                circles[(r,c)] = ("white", None)
            elif ch.upper()=="B":
                circles[(r,c)] = ("black", None)
            else:
                raise ValueError(f"Bad CIRCLES char '{ch}' at ({r},{c}).")

    # NUMBERS (optional); allow tokens with spaces for multi-digit
    _, R = read_block("NUMBERS")
    if R is not None:
        # if a row contains spaces, split on spaces; else treat as chars
        for r in range(N):
            row = R[r].strip()
            tokens = row.split() if (" " in row) else list(row)
            if len(tokens)!=N:
                raise ValueError(f"NUMBERS row {r} must have {N} tokens/chars.")
            for c,tok in enumerate(tokens):
                if tok==".": continue
                if not re.fullmatch(r"\d+", tok):
                    raise ValueError(f"NUMBERS has non-digit token '{tok}' at ({r},{c}).")
                if (r,c) in circles:
                    color, _ = circles[(r,c)]
                    circles[(r,c)] = (color, int(tok))
                # else number given on non-circle cell: ignore

    # LETTERS (optional)
    _, L = read_block("LETTERS")
    letters=None
    if L is not None:
        L = [ln.strip().replace(" ","").upper() for ln in L]
        if len(L)!=N or any(len(r)!=N or (not r.isalpha()) for r in L):
            raise ValueError("LETTERS must be N rows of A-Z.")
        letters=L

    return N, circles, letters

def parse_json_format(text):
    obj = json.loads(text)
    N = int(obj["N"])
    circles = {}
    for item in obj.get("circles", []):
        r=int(item["r"]); c=int(item["c"])
        color=item["color"].lower()
        if color not in ("white","black"):
            raise ValueError(f"Bad circle color at {(r,c)}: {color}")
        s=item.get("sum", None)
        if s is not None: s=int(s)
        circles[(r,c)] = (color, s)
    letters = obj.get("letters", None)
    if letters is not None:
        if len(letters)!=N or any(len(row)!=N for row in letters):
            raise ValueError("letters must be N strings of length N.")
        letters=[row.upper() for row in letters]
    return N, circles, letters

def load_manual_input(path=None):
    if path is None:
        return SAMPLE()
    text = open(path,"r",encoding="utf-8").read()
    try:
        return parse_json_format(text)
    except Exception:
        return parse_grid_format(text)

# ---------- Domains / propagation ----------

def restrict_by_edge(domcell, must, dir_index):
    # keep only patterns whose edge(dir_index) == must, with EMPTY allowed only if must==0
    kept=set()
    for m in domcell:
        if m==EMPTY:
            if must==0: kept.add(m)
        else:
            if edge(m,dir_index)==must:
                kept.add(m)
    return kept

def propagate(N, circles, assign, dom):
    changed=True
    while changed:
        changed=False
        # 1) Apply explicit assignments and adjacency edge-consistency
        for (r,c),m in list(assign.items()):
            # force circle constraint: circles cannot be EMPTY
            if (r,c) in circles and m==EMPTY:
                return False

            # clamp domain
            if dom[r][c]!={m}:
                dom[r][c]={m}; changed=True

            # neighbor edge consistency
            for i,(dr,dc,oppo) in enumerate(DIRS):
                nr,nc = r+dr,c+dc
                e = edge(m,i) if m!=EMPTY else 0
                if 0<=nr<N and 0<=nc<N:
                    new = restrict_by_edge(dom[nr][nc], e, oppo)
                    if not new: return False
                    if new != dom[nr][nc]:
                        dom[nr][nc]=new; changed=True
                else:
                    if e==1:
                        return False

        # 2) Circles cannot be EMPTY in domain; ensure they have some loop part
        for (r,c) in circles:
            if EMPTY in dom[r][c]:
                dom[r][c] = dom[r][c] - {EMPTY}
                if not dom[r][c]: return False
                changed=True

        # 3) Auto-assign singletons
        for r in range(N):
            for c in range(N):
                if (r,c) not in assign and len(dom[r][c])==1:
                    assign[(r,c)] = next(iter(dom[r][c]))
                    changed=True

    return True

# ---------- Search helpers ----------

def choose_var(N, dom, assign):
    best=None; size=1e9
    for r in range(N):
        for c in range(N):
            if (r,c) in assign: continue
            d=len(dom[r][c])
            if d==0: return None
            if d>1 and d<size:
                size=d; best=(r,c)
    return best

def is_loop(m):
    return m in LOOP_PARTS

def single_loop_check(N, assign, required_nodes):
    # Build graph among loop cells; ensure degree 2; ensure all required nodes are on the loop.
    parent={}
    def find(x):
        parent.setdefault(x,x)
        if parent[x]!=x: parent[x]=find(parent[x])
        return parent[x]
    def union(a,b):
        ra,rb=find(a),find(b)
        if ra!=rb: parent[rb]=ra

    used=set()
    deg=defaultdict(int)
    for r in range(N):
        for c in range(N):
            m=assign[(r,c)]
            if not is_loop(m): continue
            used.add((r,c))
            for i,(dr,dc,oppo) in enumerate(DIRS):
                if edge(m,i)==1:
                    nr,nc=r+dr,c+dc
                    if not (0<=nr<N and 0<=nc<N): return False
                    nm=assign[(nr,nc)]
                    if not is_loop(nm) or edge(nm,oppo)!=1: return False
                    union((r,c),(nr,nc))
                    deg[(r,c)]+=1
    if not used: return False
    if any(deg[u]!=2 for u in used): return False
    roots={find(u) for u in used}
    if len(roots)!=1: return False
    # Required circles must be on the loop
    for rc in required_nodes:
        if rc not in used: return False
    return True

def detect_premature_cycle(N, assign, required_nodes):
    # If a closed cycle exists that does NOT include all required nodes, prune.
    edges=defaultdict(set)
    for (r,c),m in assign.items():
        if not is_loop(m): continue
        for i,(dr,dc,oppo) in enumerate(DIRS):
            if edge(m,i)==1:
                nr,nc=r+dr,c+dc
                if (nr,nc) in assign and is_loop(assign[(nr,nc)]) and edge(assign[(nr,nc)],oppo)==1:
                    edges[(r,c)].add((nr,nc))
    visited=set()
    cycles=[]
    for node in edges:
        if node in visited: continue
        comp=set(); stack=[node]
        while stack:
            u=stack.pop()
            if u in visited: continue
            visited.add(u); comp.add(u)
            for v in edges[u]:
                if v not in visited: stack.append(v)
        # closed if all nodes have degree 2 in this component
        if all(len(edges[u])==2 for u in comp):
            cycles.append(comp)
    if not cycles: return False
    union=set().union(*cycles)
    # If there is a cycle that does not already include all required nodes, prune
    for rc in required_nodes:
        if rc not in union:
            return True
    # Multiple cycles before completion -> prune
    if len(cycles)>1: return True
    return False

# ---------- Balance constraints ----------

def run_length_from(assign, r, c, dir_index, N):
    """
    Count edges along the straight line from (r,c) in direction dir_index
    until the next turn is encountered. Length >= 1 by definition.
    Assumes assignment is complete and locally consistent.
    """
    length=0
    cr,cc=r,c
    d=dir_index
    while True:
        dr,dc,oppo = DIRS[d]
        nr,nc = cr+dr, cc+dc
        # move one step; must connect
        nm = assign[(nr,nc)]
        # count the edge we just took
        length += 1
        # if the neighbor continues straight in same direction, continue
        if (d in (0,2) and nm==VERT) or (d in (1,3) and nm==HORZ):
            cr,cc = nr,nc
            continue
        else:
            # we stop at the first turn (or if the neighbor is a corner even if
            # it turns away immediately)
            break
    return length

def check_balance_constraints(N, circles, assign):
    for (r,c),(color,sumv) in circles.items():
        m = assign[(r,c)]
        if not is_loop(m): return False  # circles must be on loop
        # determine outgoing directions at this circle
        if m==VERT:
            dirs = (0,2)  # N,S
        elif m==HORZ:
            dirs = (1,3)  # E,W
        elif m==NE:
            dirs = (0,1)  # N,E
        elif m==SE:
            dirs = (2,1)  # S,E
        elif m==SW:
            dirs = (2,3)  # S,W
        elif m==NW:
            dirs = (0,3)  # N,W
        else:
            return False
        L1 = run_length_from(assign, r, c, dirs[0], N)
        L2 = run_length_from(assign, r, c, dirs[1], N)
        if color=="white":
            if L1 != L2: return False
        else:  # black
            if L1 == L2: return False
        if sumv is not None and (L1+L2)!=sumv:
            return False
    return True

# ---------- Solver ----------

def solve_balance_loop(N, circles, time_limit=120, max_solutions=5000):
    # initial domains
    dom = [[{m for m in (EMPTY,VERT,HORZ,NE,SE,SW,NW) if valid_for_cell(N,r,c,m)} for c in range(N)] for r in range(N)]
    # circles cannot be EMPTY
    for (r,c) in circles:
        dom[r][c] -= {EMPTY}
    assign = {}
    if not propagate(N, circles, assign, dom):
        return []

    sols=[]
    start=time.time()
    required_nodes=set(circles.keys())

    def backtrack():
        if time.time()-start > time_limit:
            return
        # crude pruning: avoid premature cycles excluding circles
        if detect_premature_cycle(N, assign, required_nodes):
            return
        # completion check
        if all((r,c) in assign for r in range(N) for c in range(N)):
            if single_loop_check(N, assign, required_nodes) and check_balance_constraints(N, circles, assign):
                sols.append(dict(assign))
            return
        # choose var
        var = choose_var(N, dom, assign)
        if var is None:
            # pick any unassigned
            for r in range(N):
                for c in range(N):
                    if (r,c) not in assign:
                        var=(r,c); break
                if var: break
        r,c = var
        # prefer loop pieces over EMPTY for cells near circles? simple heuristic: if (r,c) is a circle -> loop only already.
        vals = sorted(dom[r][c], key=lambda m: (m==EMPTY, m))
        for v in vals:
            if len(sols)>=max_solutions: break
            saved_assign=dict(assign)
            saved_dom=[row[:] for row in dom]
            assign[(r,c)] = v
            if propagate(N, circles, assign, dom):
                backtrack()
            # restore
            assign.clear(); assign.update(saved_assign)
            dom[:] = saved_dom

    backtrack()
    return sols

# ---------- Extraction (optional) ----------

def extract_inside_letters(N, letters, sol):
    # Build horizontal edges
    horiz=[]
    for r in range(N):
        for c in range(N):
            m=sol[(r,c)]
            if m in LOOP_PARTS and edge(m,1)==1:
                y=r+0.5; x0=c+0.5; x1=c+1.5
                horiz.append((y,x0,x1))
    def inside_cell(r,c):
        x=c+0.5+1e-7; y=r+0.5+1e-7
        crossings=0
        for (yh,x0,x1) in horiz:
            if yh<=y: continue
            if x0 <= x < x1:
                crossings+=1
        return (crossings%2)==1
    out=[]
    for r in range(N):
        for c in range(N):
            if sol[(r,c)]==EMPTY and inside_cell(r,c):
                out.append(letters[r][c])
    return "".join(out)

def extract_unused_letters(N, letters, sol):
    out=[]
    for r in range(N):
        for c in range(N):
            if sol[(r,c)]==EMPTY:
                out.append(letters[r][c])
    return "".join(out)

# ---------- Dictionary / reporting ----------

def load_dict(path):
    if not path: return None
    try:
        with open(path,"r",encoding="utf-8") as f:
            words={ln.strip() for ln in f if ln.strip()}
        words={w for w in words if w.isalpha() and w==w.lower()}
        return words
    except Exception as e:
        print(f"[warn] Could not read dictionary '{path}': {e}")
        return None

def unique_word_frequencies(words):
    c=Counter(w.lower() for w in words)
    return sorted(c.items(), key=lambda kv: kv[0])

def report_unique_words(words, heading="\nUnique words (alpha) with frequencies:"):
    items = unique_word_frequencies(words)
    print(heading)
    if not items:
        print("(none)"); return
    for w,f in items:
        print(f"{w} {f}")

# ---------- Sample ----------

def SAMPLE():
    # Tiny 6x6 sample with a few circles; not guaranteed to be uniquely solvable.
    N=6
    # circles dict: (r,c) -> (color, sum)
    circles={
        (0,1):("white",None),
        (1,4):("black",5),
        (3,2):("white",None),
        (4,4):("black",None)
    }
    letters=[
        "ABCDEF",
        "GHIJKL",
        "MNOPQR",
        "STUVWX",
        "YZABCD",
        "EFGHIJ"
    ]
    return N, circles, letters

# ---------- CLI ----------

def main():
    ap = argparse.ArgumentParser(description="Balance Loop solver (manual input).")
    ap.add_argument("path", nargs="?", help="Path to GRID or JSON file. If omitted, uses SAMPLE().")
    ap.add_argument("--time", type=int, default=120, help="Time limit in seconds (default: 120)")
    ap.add_argument("--max", type=int, default=5000, help="Max solutions to collect (default: 5000)")
    ap.add_argument("--dict", dest="dict_path", default=None, help="Path to newline-delimited lowercase word list")
    ap.add_argument("--extract", choices=["inside","unused"], default="inside",
                    help="Letter extraction mode (default: inside)")
    ap.add_argument("--only_valid", action="store_true",
                    help="Only print boards with a dictionary-valid word (requires --dict)")
    args = ap.parse_args()

    N, circles, letters = load_manual_input(args.path)
    print(f"Loaded N={N}, circles={len(circles)}, letters={'yes' if letters else 'no'}")

    # set up initial domains
    sols = solve_balance_loop(N, circles, time_limit=args.time, max_solutions=args.max)
    print(f"Found {len(sols)} solution(s).")

    extractor = extract_inside_letters if args.extract=="inside" else extract_unused_letters
    dict_words = load_dict(args.dict_path)

    # Filter boards to those with valid word if requested
    kept=[]; kept_words=[]
    if letters:
        for sol in sols:
            w = extractor(N, letters, sol)
            if args.only_valid and dict_words is not None:
                if w and (w.lower() in dict_words):
                    kept.append((sol,w)); kept_words.append(w)
            else:
                if dict_words is None or not args.only_valid:
                    kept.append((sol,w))
                    if w: kept_words.append(w)
    else:
        kept=[(sol,"") for sol in sols]

    if args.only_valid and args.dict_path:
        print(f"\nBoards with valid words in '{args.dict_path}': {len(kept)}")
    else:
        print(f"\nBoards: {len(kept)}")

    sym={EMPTY:'.', VERT:'|', HORZ:'-', NE:'└', SE:'┌', SW:'┐', NW:'┘'}
    if not kept:
        print("(none)"); return

    for i,(sol,w) in enumerate(kept,1):
        if letters and w:
            print(f"[{i}] {w}")
        print(f"\nSolution {i} map (.'=unused, | - and corners are loop):")
        for r in range(N):
            print("".join(sym[sol[(r,c)]] for c in range(N)))

    if letters and kept_words:
        report_unique_words(kept_words)

if __name__ == "__main__":
    main()