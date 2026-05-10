
"""
Arch Madness CP-SAT candidate generator + exact validator, v2.

This fixes the main reason the first version got stuck at:
    iteration 0: UNKNOWN

Changes from v1:
  1. Region labels are labels for SCORE VALUES, not labels for individual clues.
     This removes a huge amount of symmetry and avoids forbidding two equal-score
     clues from being in the same region.
  2. Adds the local no-dangling endpoint constraint:
       every interior lattice point has 0, 2, or 4 incident arc endpoints.
  3. Runs randomized restarts and does not stop just because one CP-SAT call
     returns UNKNOWN.
  4. Every CP-SAT candidate is checked by an exact geometry validator that
     reconstructs regions, checks integer area, counts smooth perimeter pieces,
     checks all clues, and computes the final answer.

Install:
    pip install ortools

Run:
    python3 arch_madness_solver_v2.py

State convention printed by the solver:
    0 = no arc
    1 = quarter-circle centered NW
    2 = quarter-circle centered NE
    3 = quarter-circle centered SE
    4 = quarter-circle centered SW
"""

from collections import defaultdict
from math import pi
from ortools.sat.python import cp_model
import random
import sys
import time

R = C = 9
N, E, S, W = 0, 1, 2, 3

GREENS_1INDEXED = {
    (1,1),(1,4),(1,6),(1,8),
    (2,4),(2,7),(2,9),
    (3,1),(3,8),
    (4,9),
    (5,1),(5,5),
    (6,8),(6,9),
    (8,1),
    (9,2),(9,5),(9,7),(9,9),
}

CLUES_1INDEXED = {
    (1,3):21,
    (2,1):21,(2,5):27,(2,8):25,
    (3,2):27,(3,6):15,(3,9):9,
    (5,1):25,(5,4):27,(5,6):45,(5,9):9,
    (7,1):9,(7,4):63,(7,8):45,
    (8,2):63,(8,5):9,(8,9):288,
    (9,7):35,
}

greens = {(r-1, c-1) for r, c in GREENS_1INDEXED}
clues = {(r-1, c-1): v for (r, c), v in CLUES_1INDEXED.items()}
white = [(r, c) for r in range(R) for c in range(C) if (r, c) not in greens]

SCORES = sorted(set(clues.values()))
score_to_label = {s: i for i, s in enumerate(SCORES)}
label_to_score = {i: s for s, i in score_to_label.items()}
L = len(SCORES)

# State convention:
# 0 = no arc
# 1 = center NW, small piece touches N,W; large piece touches E,S
# 2 = center NE, small piece touches N,E; large piece touches S,W
# 3 = center SE, small piece touches E,S; large piece touches N,W
# 4 = center SW, small piece touches S,W; large piece touches N,E
piece_defs = {
    0: [((1, 0), {N, E, S, W})],
    1: [((0, 1), {N, W}), ((1, -1), {E, S})],
    2: [((0, 1), {N, E}), ((1, -1), {S, W})],
    3: [((0, 1), {E, S}), ((1, -1), {N, W})],
    4: [((0, 1), {S, W}), ((1, -1), {N, E})],
}

def arc_geom(r, c, st):
    """Returns endpoints and tangent directions at endpoints."""
    x, y = c, r
    if st == 1: return (x+1, y), (x, y+1), "V", "H"
    if st == 2: return (x, y), (x+1, y+1), "V", "H"
    if st == 3: return (x+1, y), (x, y+1), "H", "V"
    if st == 4: return (x, y), (x+1, y+1), "H", "V"
    raise ValueError(st)

class DSU:
    def __init__(self, n):
        self.p = list(range(n))
        self.r = [0] * n
    def find(self, x):
        while self.p[x] != x:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x
    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b:
            return False
        if self.r[a] < self.r[b]:
            a, b = b, a
        self.p[b] = a
        if self.r[a] == self.r[b]:
            self.r[a] += 1
        return True

def evaluate(grid):
    """
    Exact validator for a completed arc-state grid.
    Returns dict with:
      penalty:
        0 iff all regions have integer area and no arc has both sides in same region.
      mismatches:
        clue mismatches list.
      filled:
        score that belongs in each cell.
      answer:
        final answer if valid-ish.
    """
    piece_id = {}
    pieces = []
    majority_piece = {}

    for r in range(R):
        for c in range(C):
            st = 0 if (r, c) in greens else grid[r][c]
            for li, (area, sides) in enumerate(piece_defs[st]):
                pid = len(pieces)
                piece_id[(r, c, li)] = pid
                pieces.append((r, c, li, area, sides, st))
            majority_piece[(r, c)] = piece_id[(r, c, 0 if st == 0 else 1)]

    touch = {}
    for pid, (r, c, li, area, sides, st) in enumerate(pieces):
        for s in sides:
            touch[(r, c, s)] = pid

    dsu = DSU(len(pieces))
    for r in range(R):
        for c in range(C):
            if c + 1 < C:
                dsu.union(touch[(r, c, E)], touch[(r, c+1, W)])
            if r + 1 < R:
                dsu.union(touch[(r, c, S)], touch[(r+1, c, N)])

    region_area = defaultdict(lambda: [0, 0])
    for pid, (r, c, li, (a, b), sides, st) in enumerate(pieces):
        root = dsu.find(pid)
        region_area[root][0] += a
        region_area[root][1] += b

    cell_region = {cell: dsu.find(pid) for cell, pid in majority_piece.items()}

    # Boundary elements: only outer border segments and arcs.
    # Internal grid lines are NOT boundaries.
    elems = defaultdict(list)
    elem_data = []
    penalty = 0

    def add_elem(reg, endpoints, tangents):
        idx = len(elem_data)
        elem_data.append((reg, endpoints, tangents))
        elems[reg].append(idx)

    for r in range(R):
        for c in range(C):
            if r == 0:
                reg = dsu.find(touch[(r, c, N)])
                add_elem(reg, ((c, r), (c+1, r)), {(c, r): "H", (c+1, r): "H"})
            if c == 0:
                reg = dsu.find(touch[(r, c, W)])
                add_elem(reg, ((c, r), (c, r+1)), {(c, r): "V", (c, r+1): "V"})
            if c == C-1:
                reg = dsu.find(touch[(r, c, E)])
                add_elem(reg, ((c+1, r), (c+1, r+1)), {(c+1, r): "V", (c+1, r+1): "V"})
            if r == R-1:
                reg = dsu.find(touch[(r, c, S)])
                add_elem(reg, ((c, r+1), (c+1, r+1)), {(c, r+1): "H", (c+1, r+1): "H"})

            st = 0 if (r, c) in greens else grid[r][c]
            if st:
                small_reg = dsu.find(piece_id[(r, c, 0)])
                large_reg = dsu.find(piece_id[(r, c, 1)])
                if small_reg == large_reg:
                    penalty += 1000
                    continue
                p1, p2, t1, t2 = arc_geom(r, c, st)
                tangents = {p1: t1, p2: t2}
                add_elem(small_reg, (p1, p2), tangents)
                add_elem(large_reg, (p1, p2), tangents)

    # Smooth piece count = number of boundary-element components after gluing
    # pieces at endpoints with matching tangent direction.
    region_smooth = {}
    for reg, es in elems.items():
        edsu = DSU(len(es))
        idx = {e: i for i, e in enumerate(es)}
        at_vertex = defaultdict(list)

        for e in es:
            _, (p1, p2), tangents = elem_data[e]
            at_vertex[p1].append(e)
            at_vertex[p2].append(e)

        for v, here in at_vertex.items():
            by_tangent = defaultdict(list)
            for e in here:
                by_tangent[elem_data[e][2][v]].append(e)
            for group in by_tangent.values():
                for i in range(1, len(group)):
                    edsu.union(idx[group[0]], idx[group[i]])

        region_smooth[reg] = len({edsu.find(i) for i in range(len(es))})

    region_score = {}
    for reg, (a, b) in region_area.items():
        if b != 0:
            penalty += 100 * abs(b)
        true_area = a + b * pi / 4
        region_score[reg] = true_area * region_smooth.get(reg, 0)

    filled = [[region_score[cell_region[(r, c)]] for c in range(C)] for r in range(R)]

    mismatches = []
    for cell, clue in clues.items():
        got = filled[cell[0]][cell[1]]
        if abs(got - clue) > 1e-9:
            mismatches.append((cell, clue, got))

    answer = (
        sum(sum(row) ** 2 for row in filled)
        + sum(sum(filled[r][c] for r in range(R)) ** 2 for c in range(C))
    )

    return {
        "penalty": penalty,
        "mismatches": mismatches,
        "filled": filled,
        "answer": answer,
        "region_area": dict(region_area),
        "region_smooth": region_smooth,
        "region_score": region_score,
    }

def build_model(blocked_grids, seed):
    model = cp_model.CpModel()

    st = [[model.NewIntVar(0, 4, f"st_{r}_{c}") for c in range(C)] for r in range(R)]
    side = [[[model.NewIntVar(0, L-1, f"side_{r}_{c}_{d}") for d in range(4)] for c in range(C)] for r in range(R)]
    major = [[model.NewIntVar(0, L-1, f"major_{r}_{c}") for c in range(C)] for r in range(R)]
    small = [[model.NewIntVar(0, L-1, f"small_{r}_{c}") for c in range(C)] for r in range(R)]

    # Local transition table.
    # Columns: state, N label, E label, S label, W label, majority label, small label
    local_tuples = []

    for a in range(L):
        local_tuples.append([0, a, a, a, a, a, 0])

    for sm in range(L):
        for ma in range(L):
            if sm == ma:
                continue
            # state 1: small N/W, large E/S
            local_tuples.append([1, sm, ma, ma, sm, ma, sm])
            # state 2: small N/E, large S/W
            local_tuples.append([2, sm, sm, ma, ma, ma, sm])
            # state 3: small E/S, large N/W
            local_tuples.append([3, ma, sm, sm, ma, ma, sm])
            # state 4: small S/W, large N/E
            local_tuples.append([4, ma, ma, sm, sm, ma, sm])

    # Endpoint tuple columns:
    # state, endpoint at NW, NE, SE, SW
    endpoint_tuples = [
        [0, 0, 0, 0, 0],
        [1, 0, 1, 0, 1],
        [2, 1, 0, 1, 0],
        [3, 0, 1, 0, 1],
        [4, 1, 0, 1, 0],
    ]

    ep = {}

    for r in range(R):
        for c in range(C):
            model.AddAllowedAssignments(
                [st[r][c], side[r][c][N], side[r][c][E], side[r][c][S], side[r][c][W],
                 major[r][c], small[r][c]],
                local_tuples
            )

            eps = [model.NewBoolVar(f"ep_{r}_{c}_{k}") for k in range(4)]
            ep[(r, c)] = eps
            model.AddAllowedAssignments([st[r][c]] + eps, endpoint_tuples)

            if (r, c) in greens:
                model.Add(st[r][c] == 0)

            if (r, c) in clues:
                model.Add(major[r][c] == score_to_label[clues[(r, c)]])

            if c + 1 < C:
                model.Add(side[r][c][E] == side[r][c+1][W])
            if r + 1 < R:
                model.Add(side[r][c][S] == side[r+1][c][N])

    # No dangling arc endpoint in the interior:
    # an interior lattice point must have 0, 2, or 4 incident arc endpoints.
    for vr in range(1, R):
        for vc in range(1, C):
            terms = []
            # vertex (vr,vc) is:
            # NW of cell (vr,vc),
            # NE of cell (vr,vc-1),
            # SE of cell (vr-1,vc-1),
            # SW of cell (vr-1,vc)
            for r, c, k in [
                (vr, vc, 0),
                (vr, vc-1, 1),
                (vr-1, vc-1, 2),
                (vr-1, vc, 3),
            ]:
                if 0 <= r < R and 0 <= c < C:
                    terms.append(ep[(r, c)][k])

            cnt = model.NewIntVar(0, 4, f"cnt_{vr}_{vc}")
            model.Add(cnt == sum(terms))
            model.AddAllowedAssignments([cnt], [[0], [2], [4]])

    # Block previously tried exact arc-state grids.
    for bi, grid in enumerate(blocked_grids):
        diffs = []
        for r, c in white:
            b = model.NewBoolVar(f"block_{bi}_{r}_{c}")
            model.Add(st[r][c] != grid[r][c]).OnlyEnforceIf(b)
            model.Add(st[r][c] == grid[r][c]).OnlyEnforceIf(b.Not())
            diffs.append(b)
        model.AddBoolOr(diffs)

    # Randomized decision order to avoid repeatedly hitting the same hard branch.
    rng = random.Random(seed)
    cells = white[:]
    rng.shuffle(cells)
    decision_vars = [st[r][c] for r, c in cells]
    model.AddDecisionStrategy(decision_vars, cp_model.CHOOSE_FIRST, cp_model.SELECT_MIN_VALUE)

    return model, st

def solve(max_iterations=500, seconds_per_iteration=15):
    blocked = []
    best = None

    print("Score labels:", SCORES)
    print("Starting CP-SAT randomized candidate search.")

    for it in range(max_iterations):
        seed = 1000 + it
        model, st = build_model(blocked, seed)

        solver = cp_model.CpSolver()
        solver.parameters.max_time_in_seconds = seconds_per_iteration
        solver.parameters.num_search_workers = 8
        solver.parameters.random_seed = seed
        solver.parameters.search_branching = cp_model.FIXED_SEARCH

        t0 = time.time()
        status = solver.Solve(model)
        elapsed = time.time() - t0

        print(f"\niteration {it}: {solver.StatusName(status)} in {elapsed:.2f}s")

        if status == cp_model.UNKNOWN:
            print("  Timeout before a candidate. Continuing with a new randomized branch order.")
            print("  Stats:", solver.ResponseStats().splitlines()[0])
            continue

        if status not in (cp_model.OPTIMAL, cp_model.FEASIBLE):
            print("  CP-SAT says no more candidates under the current model.")
            return None

        grid = [[solver.Value(st[r][c]) for c in range(C)] for r in range(R)]
        result = evaluate(grid)

        clue_penalty = sum(abs(got - clue) for _, clue, got in result["mismatches"])
        quality = (len(result["mismatches"]), result["penalty"], clue_penalty)

        print(f"  validator: mismatches={len(result['mismatches'])}, "
              f"penalty={result['penalty']}, clue_abs_error={clue_penalty:.6f}")

        if best is None or quality < best[0]:
            best = (quality, grid, result)
            print("  New best candidate:")
            for row in grid:
                print("   ", " ".join(map(str, row)))

        if result["penalty"] == 0 and not result["mismatches"]:
            print("\nFOUND VALID SOLUTION")
            print("\nArc states:")
            for row in grid:
                print(" ".join(map(str, row)))

            print("\nFilled score grid:")
            filled_int = []
            for row in result["filled"]:
                int_row = [int(round(x)) for x in row]
                filled_int.append(int_row)
                print(" ".join(map(str, int_row)))

            ans = int(round(result["answer"]))
            print("\nANSWER:", ans)
            return grid, filled_int, ans

        blocked.append(grid)

    print("\nStopped without a fully valid solution.")
    if best is not None:
        quality, grid, result = best
        print("Best quality tuple was (mismatches, penalty, clue_abs_error):", quality)
    return None

if __name__ == "__main__":
    # Increase these if you let it run overnight.
    solve(max_iterations=500, seconds_per_iteration=15)
