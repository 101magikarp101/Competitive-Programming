
"""
Simplified diagonal-line SAT/CP-SAT solver for Jane Street Arch Madness.

This deliberately ignores the curve-specific constraints:
  - no smooth/continuously-differentiable edge counting
  - no quarter-circle area pi terms
  - no curve endpoint/tangent continuity

Instead, each white cell is:
  0 = no line
  1 = diagonal from NE to SW, written "/"
  2 = diagonal from NW to SE, written "\\"

Area model:
  - an undivided square contributes area 1, represented as doubled-area 2
  - a divided triangle contributes area 1/2, represented as doubled-area 1

Constraint model:
  - green cells cannot contain diagonals
  - adjacent cell pieces across a shared side must have the same region label
  - each clue cell must have at least half of the cell in a region label with that clue's score
  - for every numbered region label with score S, its doubled area A2 must divide 2*S
    equivalently, area = A2/2 is a factor of S in the sense that S/area is integral
  - unlabeled regions are allowed and unconstrained

This is intentionally a relaxed solver. It is meant to generate plausible
diagonal partitions quickly, not to prove the original curved puzzle.

Install:
    pip install ortools

Run:
    python3 arch_madness_diagonal_solver.py --seconds 10 --iterations 200

Optional:
    python3 arch_madness_diagonal_solver.py --seconds 10 --iterations 200 --draw diag_candidate.png
"""

from collections import defaultdict
from math import isclose
import argparse
import random
import time

from ortools.sat.python import cp_model

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

greens = {(r - 1, c - 1) for r, c in GREENS_1INDEXED}
clues = {(r - 1, c - 1): v for (r, c), v in CLUES_1INDEXED.items()}
white = [(r, c) for r in range(R) for c in range(C) if (r, c) not in greens]
scores = sorted(set(clues.values()))
score_counts = defaultdict(int)
for v in clues.values():
    score_counts[v] += 1


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
        a = self.find(a)
        b = self.find(b)
        if a == b:
            return False
        if self.r[a] < self.r[b]:
            a, b = b, a
        self.p[b] = a
        if self.r[a] == self.r[b]:
            self.r[a] += 1
        return True


def divisors(n):
    return [d for d in range(1, n + 1) if n % d == 0]


def build_label_pools(extra_score_copies=0, anonymous_labels=8):
    """
    Fixed labels:
      - for each clue score S, create (#clues with score S + extra_score_copies) labels
      - create anonymous labels, whose areas are unconstrained
    """
    label_score = []
    labels_by_score = defaultdict(list)

    for s in scores:
        copies = score_counts[s] + extra_score_copies
        for _ in range(copies):
            lab = len(label_score)
            label_score.append(s)
            labels_by_score[s].append(lab)

    anonymous = []
    for _ in range(anonymous_labels):
        lab = len(label_score)
        label_score.append(None)
        anonymous.append(lab)

    return label_score, labels_by_score, anonymous


def local_tuples_for_cell(K, allowed_score_labels=None):
    """
    Tuple format:
      [state, Nlabel, Elabel, Slabel, Wlabel]

    state 0:
      no diagonal, all sides same label

    state 1:
      "/" diagonal from NE to SW
      NW triangle touches N,W
      SE triangle touches S,E

    state 2:
      "\\" diagonal from NW to SE
      NE triangle touches N,E
      SW triangle touches S,W

    If allowed_score_labels is not None, this is a clue cell and at least
    one half/full piece must use a label in that set.
    """
    rows = []

    def clue_ok_state0(a):
        return allowed_score_labels is None or a in allowed_score_labels

    def clue_ok_state1(a, b):
        # slash: half a touches N/W, half b touches E/S
        return allowed_score_labels is None or a in allowed_score_labels or b in allowed_score_labels

    def clue_ok_state2(a, b):
        # backslash: half a touches N/E, half b touches S/W
        return allowed_score_labels is None or a in allowed_score_labels or b in allowed_score_labels

    for a in range(K):
        if clue_ok_state0(a):
            rows.append([0, a, a, a, a])

    for a in range(K):
        for b in range(K):
            if a == b:
                continue
            if clue_ok_state1(a, b):
                rows.append([1, a, b, b, a])
            if clue_ok_state2(a, b):
                rows.append([2, a, a, b, b])

    return rows


def area_contrib_tuples_for_label(K, lab):
    """
    Tuple format:
      [state, Nlabel, Elabel, Slabel, Wlabel, doubled_area_contribution_to_lab]
    """
    rows = set()

    # state 0: full square, all labels equal
    for a in range(K):
        contrib = 2 if a == lab else 0
        rows.add((0, a, a, a, a, contrib))

    # state 1: slash "/" with pieces N/W and E/S
    for a in range(K):
        for b in range(K):
            if a == b:
                continue
            contrib = (1 if a == lab else 0) + (1 if b == lab else 0)
            rows.add((1, a, b, b, a, contrib))

    # state 2: backslash "\\" with pieces N/E and S/W
    for a in range(K):
        for b in range(K):
            if a == b:
                continue
            contrib = (1 if a == lab else 0) + (1 if b == lab else 0)
            rows.add((2, a, a, b, b, contrib))

    return [list(x) for x in rows]


def build_model(blocked_grids, seed, extra_score_copies=0, anonymous_labels=8):
    label_score, labels_by_score, anonymous = build_label_pools(
        extra_score_copies=extra_score_copies,
        anonymous_labels=anonymous_labels,
    )
    K = len(label_score)

    model = cp_model.CpModel()

    state = [[model.NewIntVar(0, 2, f"state_{r}_{c}") for c in range(C)] for r in range(R)]
    side = [[[model.NewIntVar(0, K - 1, f"side_{r}_{c}_{d}") for d in range(4)] for c in range(C)] for r in range(R)]

    # Local state/side compatibility, with clue filtering when applicable.
    cache = {}
    for r in range(R):
        for c in range(C):
            allowed_score_labels = None
            if (r, c) in clues:
                allowed_score_labels = tuple(labels_by_score[clues[(r, c)]])

            key = allowed_score_labels
            if key not in cache:
                cache[key] = local_tuples_for_cell(
                    K,
                    allowed_score_labels=set(key) if key is not None else None,
                )

            model.AddAllowedAssignments(
                [state[r][c], side[r][c][N], side[r][c][E], side[r][c][S], side[r][c][W]],
                cache[key],
            )

            if (r, c) in greens:
                model.Add(state[r][c] == 0)

            if c + 1 < C:
                model.Add(side[r][c][E] == side[r][c + 1][W])
            if r + 1 < R:
                model.Add(side[r][c][S] == side[r + 1][c][N])

    # Area divisibility constraints for numbered labels only.
    # Anonymous labels are allowed to have any area.
    for lab, s in enumerate(label_score):
        if s is None:
            continue

        contribs = []
        rows = area_contrib_tuples_for_label(K, lab)

        for r in range(R):
            for c in range(C):
                x = model.NewIntVar(0, 2, f"area2_contrib_{lab}_{r}_{c}")
                model.AddAllowedAssignments(
                    [state[r][c], side[r][c][N], side[r][c][E], side[r][c][S], side[r][c][W], x],
                    rows,
                )
                contribs.append(x)

        area2 = model.NewIntVar(0, 2 * R * C, f"area2_label_{lab}")
        model.Add(area2 == sum(contribs))

        allowed_area2 = [0] + divisors(2 * s)
        model.AddAllowedAssignments([area2], [[x] for x in allowed_area2])

    # Block previously validated/failed exact diagonal grids.
    for bi, grid in enumerate(blocked_grids):
        diffs = []
        for r, c in white:
            b = model.NewBoolVar(f"block_{bi}_{r}_{c}")
            model.Add(state[r][c] != grid[r][c]).OnlyEnforceIf(b)
            model.Add(state[r][c] == grid[r][c]).OnlyEnforceIf(b.Not())
            diffs.append(b)
        model.AddBoolOr(diffs)

    # Randomized fixed-search ordering.
    rng = random.Random(seed)
    cells = white[:]
    rng.shuffle(cells)
    decision_vars = [state[r][c] for r, c in cells]
    model.AddDecisionStrategy(decision_vars, cp_model.CHOOSE_FIRST, cp_model.SELECT_MIN_VALUE)

    return model, state, label_score, labels_by_score


def exact_components_for_grid(grid):
    """
    Reconstruct the true diagonal regions from just the diagonal-state grid.
    Returns:
      component_area2: root -> doubled area
      clue_candidates: clue_cell -> list of component roots that contain at least half the cell
      cell_majority_candidates: cell -> list of component roots that contain at least half the cell
    """
    piece_id = {}
    pieces = []
    touch = {}

    def add_piece(r, c, area2, sides):
        pid = len(pieces)
        pieces.append((r, c, area2, set(sides)))
        return pid

    for r in range(R):
        for c in range(C):
            st = 0 if (r, c) in greens else grid[r][c]

            if st == 0:
                pid = add_piece(r, c, 2, [N, E, S, W])
                piece_id[(r, c, 0)] = pid
                for d in [N, E, S, W]:
                    touch[(r, c, d)] = pid

            elif st == 1:
                # slash: NE-SW diagonal
                # piece 0: NW triangle touches N,W
                # piece 1: SE triangle touches E,S
                p0 = add_piece(r, c, 1, [N, W])
                p1 = add_piece(r, c, 1, [E, S])
                piece_id[(r, c, 0)] = p0
                piece_id[(r, c, 1)] = p1
                touch[(r, c, N)] = p0
                touch[(r, c, W)] = p0
                touch[(r, c, E)] = p1
                touch[(r, c, S)] = p1

            elif st == 2:
                # backslash: NW-SE diagonal
                # piece 0: NE triangle touches N,E
                # piece 1: SW triangle touches S,W
                p0 = add_piece(r, c, 1, [N, E])
                p1 = add_piece(r, c, 1, [S, W])
                piece_id[(r, c, 0)] = p0
                piece_id[(r, c, 1)] = p1
                touch[(r, c, N)] = p0
                touch[(r, c, E)] = p0
                touch[(r, c, S)] = p1
                touch[(r, c, W)] = p1

            else:
                raise ValueError(f"bad state {st}")

    dsu = DSU(len(pieces))

    for r in range(R):
        for c in range(C):
            if c + 1 < C:
                dsu.union(touch[(r, c, E)], touch[(r, c + 1, W)])
            if r + 1 < R:
                dsu.union(touch[(r, c, S)], touch[(r + 1, c, N)])

    component_area2 = defaultdict(int)
    for pid, (_, _, area2, _) in enumerate(pieces):
        component_area2[dsu.find(pid)] += area2

    clue_candidates = {}
    cell_candidates = {}

    for r in range(R):
        for c in range(C):
            st = 0 if (r, c) in greens else grid[r][c]
            if st == 0:
                comps = [dsu.find(piece_id[(r, c, 0)])]
            else:
                comps = sorted({
                    dsu.find(piece_id[(r, c, 0)]),
                    dsu.find(piece_id[(r, c, 1)]),
                })
            cell_candidates[(r, c)] = comps
            if (r, c) in clues:
                clue_candidates[(r, c)] = comps

    return dict(component_area2), clue_candidates, cell_candidates


def assign_clues_to_components(component_area2, clue_candidates):
    """
    For each clue cell, choose one of its >= half components.
    A component assigned clue score S must satisfy area2 | 2*S.
    If multiple clues choose the same component, their scores must agree.

    Returns:
      (ok, component_score_assignment)
    """
    items = sorted(clue_candidates.items(), key=lambda kv: len(kv[1]))
    assigned = {}

    def rec(i):
        if i == len(items):
            return True

        cell, comps = items[i]
        s = clues[cell]

        for comp in comps:
            a2 = component_area2[comp]
            if (2 * s) % a2 != 0:
                continue

            old = assigned.get(comp)
            if old is not None and old != s:
                continue

            if old is None:
                assigned[comp] = s

            if rec(i + 1):
                return True

            if old is None:
                del assigned[comp]

        return False

    ok = rec(0)
    return ok, dict(assigned)


def simplified_fill_grid(grid, component_score_assignment, cell_candidates):
    """
    Fill cells whose >= half component has a known score.
    Diagonal cells can have two half components; if both are scored and differ,
    show "a/b"; if neither is scored, show ".".
    """
    out = []
    for r in range(R):
        row = []
        for c in range(C):
            vals = sorted({
                component_score_assignment[x]
                for x in cell_candidates[(r, c)]
                if x in component_score_assignment
            })
            if len(vals) == 0:
                row.append(".")
            elif len(vals) == 1:
                row.append(str(vals[0]))
            else:
                row.append("/".join(map(str, vals)))
        out.append(row)
    return out


def print_grid(grid):
    symbols = {0: ".", 1: "/", 2: "\\"}
    for row in grid:
        print(" ".join(symbols[x] for x in row))


def draw_grid(grid, out_path):
    import matplotlib.pyplot as plt
    from matplotlib.patches import Rectangle

    fig, ax = plt.subplots(figsize=(9, 9))

    greens_1 = {(r + 1, c + 1) for r, c in greens}
    clues_1 = {(r + 1, c + 1): v for (r, c), v in clues.items()}

    for r1 in range(1, R + 1):
        for c1 in range(1, C + 1):
            x, y = c1 - 1, R - r1
            face = "#b8e6b1" if (r1, c1) in greens_1 else "white"
            ax.add_patch(Rectangle((x, y), 1, 1, facecolor=face, edgecolor="black", linewidth=1.2))

            st = grid[r1 - 1][c1 - 1]
            if st == 1:
                ax.plot([x + 1, x], [y + 1, y], linewidth=3)
            elif st == 2:
                ax.plot([x, x + 1], [y + 1, y], linewidth=3)

            if (r1, c1) in clues_1:
                ax.text(x + 0.5, y + 0.58, str(clues_1[(r1, c1)]),
                        ha="center", va="center", fontsize=18)

    ax.add_patch(Rectangle((0, 0), C, R, fill=False, edgecolor="black", linewidth=3))
    ax.set_xlim(-0.2, C + 0.2)
    ax.set_ylim(-0.2, R + 0.2)
    ax.set_aspect("equal")
    ax.axis("off")
    ax.set_title("Simplified diagonal candidate")
    plt.savefig(out_path, dpi=200, bbox_inches="tight")
    plt.close(fig)


def solve(args):
    blocked = []
    best_quality = None
    best_grid = None
    best_assignment = None
    best_fill = None

    print("Scores:", scores)
    print("Score counts:", dict(score_counts))
    print(f"Relaxed diagonal solver: iterations={args.iterations}, seconds={args.seconds}")

    for it in range(args.iterations):
        seed = args.seed + it
        model, state, label_score, labels_by_score = build_model(
            blocked,
            seed=seed,
            extra_score_copies=args.extra_score_copies,
            anonymous_labels=args.anonymous_labels,
        )

        solver = cp_model.CpSolver()
        solver.parameters.max_time_in_seconds = args.seconds
        solver.parameters.num_search_workers = args.workers
        solver.parameters.random_seed = seed
        solver.parameters.search_branching = cp_model.FIXED_SEARCH

        t0 = time.time()
        status = solver.Solve(model)
        elapsed = time.time() - t0

        print(f"\niteration {it}: {solver.StatusName(status)} in {elapsed:.2f}s")

        if status == cp_model.UNKNOWN:
            print("  No CP-SAT candidate within this time slice; trying a new seed.")
            continue

        if status not in (cp_model.OPTIMAL, cp_model.FEASIBLE):
            print("  No more candidates under this relaxed model.")
            break

        grid = [[solver.Value(state[r][c]) for c in range(C)] for r in range(R)]

        component_area2, clue_candidates, cell_candidates = exact_components_for_grid(grid)
        ok, assignment = assign_clues_to_components(component_area2, clue_candidates)

        # quality = number of clues that can be individually satisfied by at least one candidate component
        individually_ok = 0
        for cell, comps in clue_candidates.items():
            s = clues[cell]
            if any((2 * s) % component_area2[x] == 0 for x in comps):
                individually_ok += 1

        quality = (-int(ok), -individually_ok, len(component_area2))

        print(f"  exact diagonal validation: global_ok={ok}, individually_ok={individually_ok}/{len(clues)}, components={len(component_area2)}")

        if best_quality is None or quality < best_quality:
            best_quality = quality
            best_grid = grid
            best_assignment = assignment
            best_fill = simplified_fill_grid(grid, assignment, cell_candidates)
            print("  New best diagonal grid:")
            print_grid(grid)
            print("  Known simplified fill grid:")
            for row in best_fill:
                print(" ".join(f"{x:>5}" for x in row))

        if ok:
            print("\nFOUND RELAXED DIAGONAL SOLUTION")
            print("\nDiagonal grid:")
            print_grid(grid)

            fill = simplified_fill_grid(grid, assignment, cell_candidates)
            print("\nKnown simplified fill grid:")
            for row in fill:
                print(" ".join(f"{x:>5}" for x in row))

            if args.draw:
                draw_grid(grid, args.draw)
                print("\nWrote image:", args.draw)

            return grid, assignment, fill

        blocked.append(grid)

    print("\nStopped without a globally valid relaxed diagonal solution.")
    if best_grid is not None:
        print("Best diagonal grid:")
        print_grid(best_grid)
        if args.draw:
            draw_grid(best_grid, args.draw)
            print("Wrote best image:", args.draw)

    return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--iterations", type=int, default=200)
    parser.add_argument("--seconds", type=float, default=10.0)
    parser.add_argument("--workers", type=int, default=8)
    parser.add_argument("--seed", type=int, default=12345)
    parser.add_argument("--extra-score-copies", type=int, default=0)
    parser.add_argument("--anonymous-labels", type=int, default=8)
    parser.add_argument("--draw", type=str, default="")
    args = parser.parse_args()
    solve(args)


if __name__ == "__main__":
    main()
