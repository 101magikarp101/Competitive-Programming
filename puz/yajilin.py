#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Hybrid Yajilin/Castle (black) 10x10 — direct brute force with aggressive backtracking
# Outside clues (arrows point into the grid):
# left:  1105030403
# right: 1451510303
# up:    2452111415
# down:  2254133453

from collections import deque

N = 10

L = [int(c) for c in "1105030403"]
R = [int(c) for c in "1451510303"]
U = [int(c) for c in "2452111415"]
D = [int(c) for c in "2254133453"]

# Which rows/cols are (forced pair) vs (equal-digit)
row_forced = [None]*N
row_equal  = [None]*N
for i in range(N):
    if L[i] != R[i]: row_forced[i] = {L[i], R[i]}
    else:            row_equal[i]  = L[i]

col_forced = [None]*N
col_equal  = [None]*N
for j in range(N):
    if U[j] != D[j]: col_forced[j] = {U[j], D[j]}
    else:            col_equal[j]  = U[j]

# All 10-bit row masks with no adjacent 1s (1=black)
row_patterns_by_count = {k: [] for k in range(0, 6)}  # max 5 blacks in a row without touching
for m in range(1<<N):
    if m & (m<<1):      # adjacent blacks → invalid
        continue
    k = m.bit_count()
    if k <= 5:
        row_patterns_by_count[k].append(m)

def row_max_horizontal_pairs(mask:int)->int:
    # count of j where (j,j+1) are both white → upper bound for row’s horizontal loop edges
    cnt=0
    for j in range(N-1):
        if ((mask>>j)&1)==0 and ((mask>>(j+1))&1)==0:
            cnt+=1
    return cnt

# ---------- Phase B: place loop through all white cells with DSU/rollback ----------
Udir,Rdir,Ddir,Ldir = 0,1,2,3
bit = [1<<Udir,1<<Rdir,1<<Ddir,1<<Ldir]
DIR = [(-1,0),(0,1),(1,0),(0,-1)]
OPP = [2,3,0,1]

ALL_SHAPES = [
    bit[Udir]|bit[Ddir],  # │
    bit[Ldir]|bit[Rdir],  # ─
    bit[Udir]|bit[Rdir],  # └
    bit[Rdir]|bit[Ddir],  # ┌
    bit[Ddir]|bit[Ldir],  # ┐
    bit[Ldir]|bit[Udir],  # ┘
]
shape_to_char = {
    (bit[Udir]|bit[Ddir]): "│",
    (bit[Ldir]|bit[Rdir]): "─",
    (bit[Udir]|bit[Rdir]): "└",
    (bit[Rdir]|bit[Ddir]): "┌",
    (bit[Ddir]|bit[Ldir]): "┐",
    (bit[Ldir]|bit[Udir]): "┘",
}

def inb(i,j): return 0<=i<N and 0<=j<N

class RollbackDSU:
    def __init__(self, n):
        self.p=list(range(n)); self.r=[0]*n; self.stk=[]
    def f(self,x):
        while self.p[x]!=x:
            x=self.p[x]
        return x
    def u(self,a,b):
        ra,rb=self.f(a),self.f(b)
        if ra==rb:
            self.stk.append(("noop",))
            return False
        if self.r[ra]<self.r[rb]:
            ra,rb=rb,ra
        self.stk.append(("link",rb,self.p[rb],ra,self.r[ra]))
        self.p[rb]=ra
        if self.r[ra]==self.r[rb]: self.r[ra]+=1
        return True
    def rollback(self):
        if not self.stk: return
        rec=self.stk.pop()
        if rec[0]=="noop": return
        _,rb,oldp,ra,oldr=rec
        self.p[rb]=oldp; self.r[ra]=oldr

def solve_loop_and_emit(black, row_b_targets, col_c_targets, out_solutions):
    # per row/col black counts
    b_row=[sum(1 for j in range(N) if black[i][j]) for i in range(N)]
    c_col=[sum(1 for i in range(N) if black[i][j]) for j in range(N)]
    # match targets set in Phase A (only for forced rows/cols)
    for i in range(N):
        if row_b_targets[i] is not None and b_row[i]!=row_b_targets[i]: return
    for j in range(N):
        if col_c_targets[j] is not None and c_col[j]!=col_c_targets[j]: return

    # Derive horizontal/vertical edge targets for unequal lines
    h_target=[None]*N
    v_target=[None]*N
    for i in range(N):
        if row_forced[i] is not None:
            a,b = list(row_forced[i])
            if b_row[i]==a: h_target[i]=b
            elif b_row[i]==b: h_target[i]=a
            else: return
    for j in range(N):
        if col_forced[j] is not None:
            a,b = list(col_forced[j])
            if c_col[j]==a: v_target[j]=b
            elif c_col[j]==b: v_target[j]=a
            else: return

    # grid state
    state=[[-2]*N for _ in range(N)]   # -1 black, -2 white-unassigned, else mask
    need =[[0]*N  for _ in range(N)]
    for i in range(N):
        for j in range(N):
            if black[i][j]: state[i][j]=-1

    # edge availability caches
    def can_pair_h(i,j):
        if black[i][j] or black[i][j+1]: return False
        si,sj=state[i][j],state[i][j+1]
        if si>=0 and sj>=0:
            return (si & bit[Rdir]) and (sj & bit[Ldir])
        if si>=0 and not(si & bit[Rdir]): return False
        if sj>=0 and not(sj & bit[Ldir]): return False
        return True
    def can_pair_v(i,j):
        if black[i][j] or black[i+1][j]: return False
        si,sj=state[i][j],state[i+1][j]
        if si>=0 and sj>=0:
            return (si & bit[Ddir]) and (sj & bit[Udir])
        if si>=0 and not(si & bit[Ddir]): return False
        if sj>=0 and not(sj & bit[Udir]): return False
        return True

    h_used=[0]*N; v_used=[0]*N
    h_can=[0]*N;  v_can=[0]*N
    h_cache={};   v_cache={}
    for i in range(N):
        cnt=0
        for j in range(N-1):
            ok=can_pair_h(i,j); h_cache[(i,j)]=ok
            if ok: cnt+=1
        h_can[i]=cnt
    for j in range(N):
        cnt=0
        for i in range(N-1):
            ok=can_pair_v(i,j); v_cache[(i,j)]=ok
            if ok: cnt+=1
        v_can[j]=cnt

    def refresh_pairs(i,j):
        if j-1>=0:
            bef=h_cache[(i,j-1)]; now=can_pair_h(i,j-1)
            if bef and not now: h_can[i]-=1
            if not bef and now: h_can[i]+=1
            h_cache[(i,j-1)]=now
        if j<N-1:
            bef=h_cache[(i,j)];   now=can_pair_h(i,j)
            if bef and not now: h_can[i]-=1
            if not bef and now: h_can[i]+=1
            h_cache[(i,j)]=now
        if i-1>=0:
            bef=v_cache[(i-1,j)]; now=can_pair_v(i-1,j)
            if bef and not now: v_can[j]-=1
            if not bef and now: v_can[j]+=1
            v_cache[(i-1,j)]=now
        if i<N-1:
            bef=v_cache[(i,j)];   now=can_pair_v(i,j)
            if bef and not now: v_can[j]-=1
            if not bef and now: v_can[j]+=1
            v_cache[(i,j)]=now

    # DSU + degree
    idx=[[-1]*N for _ in range(N)]
    id2=[]; wid=0
    for i in range(N):
        for j in range(N):
            if not black[i][j]:
                idx[i][j]=wid; id2.append((i,j)); wid+=1
    dsu=RollbackDSU(wid); degree=[0]*wid; assigned=0

    # quick impossibility: any white cell with <2 white neighbors
    for i in range(N):
        for j in range(N):
            if not black[i][j]:
                k=0
                for d in range(4):
                    ni,nj=i+DIR[d][0],j+DIR[d][1]
                    if inb(ni,nj) and not black[ni][nj]: k+=1
                if k<2: return

    mask_dirs={m:[d for d in range(4) if m & bit[d]] for m in ALL_SHAPES}

    def counts_ok():
        for i in range(N):
            if h_target[i] is not None:
                t=h_target[i]
                if h_used[i]>t or h_used[i]+h_can[i]<t: return False
        for j in range(N):
            if v_target[j] is not None:
                t=v_target[j]
                if v_used[j]>t or v_used[j]+v_can[j]<t: return False
        return True

    def pick_next():
        best=None; score=-1
        for i in range(N):
            for j in range(N):
                if state[i][j]==-2:
                    nb=bin(need[i][j]).count("1")
                    blocked=sum(1 for d in range(4) if not inb(i+DIR[d][0],j+DIR[d][1]) or black[i+DIR[d][0]][j+DIR[d][1]])
                    s=nb*10+blocked
                    if s>score: score=s; best=(i,j)
        return best

    def assign(i,j,m):
        nonlocal assigned
        if (m & need[i][j]) != need[i][j]: return None
        for d in mask_dirs[m]:
            ni,nj=i+DIR[d][0],j+DIR[d][1]
            if not inb(ni,nj) or black[ni][nj]: return None

        state[i][j]=m; assigned+=1
        changed=[]; used=[]; deg_inc=[]; dsu_ops=0

        for d in mask_dirs[m]:
            ni,nj=i+DIR[d][0],j+DIR[d][1]; opp=OPP[d]
            if state[ni][nj]>=0:
                if state[ni][nj] & bit[opp]:
                    if (i,j)<(ni,nj):
                        if d in (Ldir,Rdir): h_used[i]+=1; used.append(("h",i))
                        else:               v_used[j]+=1; used.append(("v",j))
                    a=idx[i][j]; b=idx[ni][nj]
                    degree[a]+=1; deg_inc.append(a)
                    degree[b]+=1; deg_inc.append(b)
                    ra,rb=dsu.f(a),dsu.f(b)
                    if ra==rb:
                        if wid-assigned>0:
                            for t,k in reversed(used):
                                if t=="h": h_used[k]-=1
                                else:      v_used[k]-=1
                            for x in deg_inc: degree[x]-=1
                            state[i][j]=-2; assigned-=1
                            return None
                        dsu.u(a,b); dsu_ops+=1
                    else:
                        dsu.u(a,b); dsu_ops+=1
                else:
                    for t,k in reversed(used):
                        if t=="h": h_used[k]-=1
                        else:      v_used[k]-=1
                    for x in deg_inc: degree[x]-=1
                    state[i][j]=-2; assigned-=1
                    return None
            else:
                old=need[ni][nj]; need[ni][nj]|=bit[opp]
                if need[ni][nj]!=old: changed.append((ni,nj,old))

        impacted={(i,j)}
        for d in range(4):
            ni,nj=i+DIR[d][0],j+DIR[d][1]
            if inb(ni,nj): impacted.add((ni,nj))
        for (ii,jj) in impacted: refresh_pairs(ii,jj)

        if not counts_ok():
            for (ii,jj,old) in reversed(changed): need[ii][jj]=old
            for t,k in reversed(used):
                if t=="h": h_used[k]-=1
                else:      v_used[k]-=1
            for x in deg_inc: degree[x]-=1
            for _ in range(dsu_ops): dsu.rollback()
            state[i][j]=-2; assigned-=1
            for (ii,jj) in impacted: refresh_pairs(ii,jj)
            return None

        return (changed, used, deg_inc, dsu_ops, impacted)

    def dfs():
        if assigned==wid:
            # count exact h_row / v_col
            h_row=[0]*N; v_col=[0]*N
            for i in range(N):
                for j in range(N-1):
                    if not black[i][j] and not black[i][j+1]:
                        if (state[i][j] & bit[Rdir]) and (state[i][j+1] & bit[Ldir]):
                            h_row[i]+=1
            for j in range(N):
                for i in range(N-1):
                    if not black[i][j] and not black[i+1][j]:
                        if (state[i][j] & bit[Ddir]) and (state[i+1][j] & bit[Udir]):
                            v_col[j]+=1
            # equal-digit lines: either black==d OR edges==d
            for i in range(N):
                d=row_equal[i]
                if d is not None:
                    if not (b_row[i]==d or h_row[i]==d): return
            for j in range(N):
                d=col_equal[j]
                if d is not None:
                    if not (c_col[j]==d or v_col[j]==d): return

            # build printable solution
            def rclass(i,val):
                y=(val==b_row[i]); c=(val==h_row[i])
                return "Y/C" if y and c else ("Y" if y else ("C" if c else "-"))
            def cclass(j,val):
                y=(val==c_col[j]); c=(val==v_col[j])
                return "Y/C" if y and c else ("Y" if y else ("C" if c else "-"))

            grid=[[" "]*N for _ in range(N)]
            for i in range(N):
                for j in range(N):
                    grid[i][j] = "#" if black[i][j] else shape_to_char[state[i][j]]
            lines=[]
            lines.append("  Up:   " + " ".join(str(x) for x in U))
            lines.append("        " + " ".join(f"{cclass(j,U[j]):>3}" for j in range(N)))
            for i in range(N):
                li,ri=L[i],R[i]
                lines.append(f"L {li:>2} [{rclass(i,li):>3}]  {''.join(grid[i])}  [{rclass(i,ri):<3}] {ri:>2} R")
            lines.append("  Down: " + " ".join(str(x) for x in D))
            lines.append("        " + " ".join(f"{cclass(j,D[j]):>3}" for j in range(N)))
            out_solutions.append("\n".join(lines))
            return

        i,j = pick_next()
        need_mask=need[i][j]
        cand=[]
        for m in ALL_SHAPES:
            if (m & need_mask) != need_mask: continue
            ok=True
            for d in (0,1,2,3):
                if m & bit[d]:
                    ni,nj=i+DIR[d][0], j+DIR[d][1]
                    if not inb(ni,nj) or black[ni][nj]: ok=False; break
                    if state[ni][nj]>=0 and not (state[ni][nj] & bit[OPP[d]]): ok=False; break
            if ok: cand.append(m)
        cand.sort(key=lambda m: 0 if m in (bit[Udir]|bit[Ddir], bit[Ldir]|bit[Rdir]) else 1)

        for m in cand:
            rec=assign(i,j,m)
            if rec is None: continue
            dfs()
            changed, used, deg_inc, dsu_ops, impacted = rec
            for (ii,jj,old) in reversed(changed): need[ii][jj]=old
            for t,k in reversed(used):
                if t=="h": h_used[k]-=1
                else:      v_used[k]-=1
            for x in deg_inc: degree[x]-=1
            for _ in range(dsu_ops): dsu.rollback()
            state[i][j]=-2; assigned-=1
            for (ii,jj) in impacted: refresh_pairs(ii,jj)

    dfs()

# ---------- Phase A: enumerate black rows with pruning ----------
solutions = []

def build_black_grid_from_row_masks(row_masks):
    g=[[False]*N for _ in range(N)]
    for i,m in enumerate(row_masks):
        for j in range(N):
            if (m>>j)&1: g[i][j]=True
    return g

def direct_bruteforce():
    row_masks=[None]*N
    col_counts=[0]*N
    prev_mask=0
    v_pairs_so_far=[0]*N  # number of vertical white-white adjacencies decided so far per column

    def dfs_row(i):
        nonlocal prev_mask, v_pairs_so_far
        if i==N:
            total_black=sum(col_counts)
            if (N*N - total_black) % 2 != 0: return
            # forced unequal cols must end with c_col ∈ set
            for j in range(N):
                if col_forced[j] is not None and col_counts[j] not in col_forced[j]:
                    return
            # choose targets for rows/cols to pass to loop phase
            row_b_targets=[None]*N
            col_c_targets=[None]*N
            for r in range(N):
                if row_forced[r] is not None:
                    # black count already chosen implicitly; we only check consistency in phase B
                    row_b_targets[r] = row_masks[r].bit_count()
            for j in range(N):
                if col_forced[j] is not None:
                    col_c_targets[j] = col_counts[j]
            black = build_black_grid_from_row_masks(row_masks)
            solve_loop_and_emit(black, row_b_targets, col_c_targets, solutions)
            return

        # candidate row masks
        candidates=[]
        if row_forced[i] is not None:
            a,b = list(row_forced[i])
            for k in (a,b):
                candidates += row_patterns_by_count.get(k, [])
        else:
            for k,pats in row_patterns_by_count.items():
                candidates += pats

        d_equal = row_equal[i]
        rem_rows_after = N-1 - i

        for m in candidates:
            # vertical non-adj to previous row
            if i>0 and (m & prev_mask): continue

            # equal row: if not b==d, must be able to host h==d
            if d_equal is not None:
                bi = m.bit_count()
                if bi != d_equal and row_max_horizontal_pairs(m) < d_equal:
                    continue

            ok=True
            new_cols = col_counts[:]
            # pairs contributed by boundary (i-1,i)
            boundary_pairs = [0]*N
            if i>0:
                for j in range(N):
                    if ((prev_mask>>j)&1)==0 and ((m>>j)&1)==0:
                        boundary_pairs[j]=1

            for j in range(N):
                if (m>>j)&1: new_cols[j]+=1
                if new_cols[j] > 5: ok=False; break

                # unequal column: c must be able to become one of {U,D}
                if col_forced[j] is not None:
                    a,b = list(col_forced[j])
                    rem_max = new_cols[j] + (rem_rows_after + 1)//2
                    if not (new_cols[j] <= a <= rem_max or new_cols[j] <= b <= rem_max):
                        ok=False; break

                # equal column: if c cannot become d, then v must be able to become d (capacity upper bound)
                if col_equal[j] is not None:
                    dcol = col_equal[j]
                    rem_max_c = new_cols[j] + (rem_rows_after + 1)//2
                    can_c = (new_cols[j] <= dcol <= rem_max_c)
                    if not can_c:
                        v_up = v_pairs_so_far[j] + boundary_pairs[j] + (N-1 - i)
                        if v_up < dcol:
                            ok=False; break

            if not ok: continue

            # unequal row: its h is fixed to the "other" digit; ensure row can host that many horizontal edges at all
            if row_forced[i] is not None:
                a,b = list(row_forced[i])
                bi = m.bit_count()
                hi = b if bi==a else a
                if hi > row_max_horizontal_pairs(m):   # impossible to realize later
                    continue

            # commit
            prev_save = prev_mask
            prev_mask = m
            old_cols = col_counts[:]
            col_counts[:] = new_cols
            old_v = v_pairs_so_far[:]
            if i>0:
                for j in range(N):
                    v_pairs_so_far[j] += boundary_pairs[j]

            row_masks[i]=m
            dfs_row(i+1)

            # rollback
            row_masks[i]=None
            v_pairs_so_far = old_v
            col_counts[:] = old_cols
            prev_mask = prev_save

    dfs_row(0)

def main():
    direct_bruteforce()
    if not solutions:
        print("No solutions found.")
    else:
        print(f"Found {len(solutions)} solution(s).\n")
        for k,s in enumerate(solutions,1):
            print(f"Solution {k}:\n{s}\n" + "-"*80)

if __name__ == "__main__":
    main()
