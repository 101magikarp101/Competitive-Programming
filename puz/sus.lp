% Hybrid Yajilin × Castle (black) — 10x10 ASP (clingo)
% Loop through all non-black cells; black cells do not touch orthogonally.
% For each line, the two outside numbers are the unordered pair {black count, oriented loop length}.
% Oriented length = total # of used edges along that orientation in that line
% (horizontal in rows for Left/Right; vertical in columns for Up/Down).

% ---------- domain ----------
row(1..10).  col(1..10).
adj_h(R,C) :- row(R), col(C), C < 10.   % horizontal pair (R,C)-(R,C+1)
adj_v(R,C) :- row(R), col(C), R < 10.   % vertical   pair (R,C)-(R+1,C)

% ---------- instance: clues ----------
% left  = 1105030403
% right = 1451510303
% up    = 2452111415
% down  = 2254133453

left(1,1).  right(1,1).
left(2,1).  right(2,4).
left(3,0).  right(3,5).
left(4,5).  right(4,1).
left(5,0).  right(5,5).
left(6,3).  right(6,1).
left(7,0).  right(7,0).
left(8,4).  right(8,3).
left(9,0).  right(9,0).
left(10,3). right(10,3).

up(1,2).  down(1,2).
up(2,4).  down(2,2).
up(3,5).  down(3,5).
up(4,2).  down(4,4).
up(5,1).  down(5,1).
up(6,1).  down(6,3).
up(7,1).  down(7,3).
up(8,4).  down(8,4).
up(9,1).  down(9,5).
up(10,5). down(10,3).

% classify unequal/equal lines
row_pair(R,A,B)  :- left(R,A), right(R,B), A != B.
row_equal(R,D)   :- left(R,D), right(R,D).
col_pair(C,A,B)  :- up(C,A),   down(C,B),  A != B.
col_equal(C,D)   :- up(C,D),   down(C,D).

% ---------- decision: black cells ----------
{ black(R,C) } :- row(R), col(C).
white(R,C) :- row(R), col(C), not black(R,C).

% no orthogonal adjacency of blacks
:- black(R,C), black(R,C+1).
:- black(R,C), black(R+1,C).

% ---------- decision: loop edges (only on white-white adjacencies) ----------
{ use_h(R,C) } :- adj_h(R,C), white(R,C), white(R,C+1).
{ use_v(R,C) } :- adj_v(R,C), white(R,C), white(R+1,C).

% ---------- degree-2 at every white cell ----------
inc_hl(R,C) :- white(R,C), adj_h(R,C),     use_h(R,C).        % to right
inc_hr(R,C) :- white(R,C), C>1, adj_h(R,C-1), use_h(R,C-1).   % from left
inc_vu(R,C) :- white(R,C), adj_v(R,C),     use_v(R,C).        % to down
inc_vd(R,C) :- white(R,C), R>1, adj_v(R-1,C), use_v(R-1,C).   % from up
:- white(R,C),
   2 != #count { t : inc_hl(R,C);
                 t : inc_hr(R,C);
                 t : inc_vu(R,C);
                 t : inc_vd(R,C) }.

% ---------- single-loop (connectivity) ----------
1 { seed(R,C) : white(R,C) } 1.
reach(R,C) :- seed(R,C).
reach(R,C+1) :- reach(R,C),   adj_h(R,C), use_h(R,C).
reach(R,C)   :- reach(R,C+1), adj_h(R,C), use_h(R,C).
reach(R+1,C) :- reach(R,C),   adj_v(R,C), use_v(R,C).
reach(R,C)   :- reach(R+1,C), adj_v(R,C), use_v(R,C).
:- white(R,C), not reach(R,C).

% ---------- per-line counts ----------
% row black & horizontal-edge totals
% (horizontal edges = total length along row)
% column black & vertical-edge totals
bcount(R,K) :- row(R), K = #count { C : col(C),           black(R,C) }.
hcount(R,K) :- row(R), K = #count { C : adj_h(R,C),       use_h(R,C) }.
ccount(C,K) :- col(C), K = #count { R : row(R),           black(R,C) }.
vcount(C,K) :- col(C), K = #count { R : adj_v(R,C),       use_v(R,C) }.

% ---------- clue satisfaction (pair = pair) ----------
% Unequal rows: {b,h} must equal {left,right}
row_ok(R) :- row_pair(R,A,B), bcount(R,A), hcount(R,B).
row_ok(R) :- row_pair(R,A,B), bcount(R,B), hcount(R,A).
:- row_pair(R,_,_), not row_ok(R).

% Equal rows: either b==d OR h==d (existential over which clue is Y or C)
row_ok(R) :- row_equal(R,D), bcount(R,D).
row_ok(R) :- row_equal(R,D), hcount(R,D).
:- row_equal(R,_), not row_ok(R).

% Columns (same)
col_ok(C) :- col_pair(C,A,B), ccount(C,A), vcount(C,B).
col_ok(C) :- col_pair(C,A,B), ccount(C,B), vcount(C,A).
:- col_pair(C,_,_), not col_ok(C).

col_ok(C) :- col_equal(C,D), ccount(C,D).
col_ok(C) :- col_equal(C,D), vcount(C,D).
:- col_equal(C,_), not col_ok(C).

% ---------- pretty printing ----------
hasR(R,C) :- white(R,C), adj_h(R,C),     use_h(R,C).
hasL(R,C) :- white(R,C), C>1,  adj_h(R,C-1), use_h(R,C-1).
hasD(R,C) :- white(R,C), adj_v(R,C),     use_v(R,C).
hasU(R,C) :- white(R,C), R>1,  adj_v(R-1,C), use_v(R-1,C).

pipe(R,C,"│") :- white(R,C), hasU(R,C), hasD(R,C), not hasL(R,C), not hasR(R,C).
pipe(R,C,"─") :- white(R,C), hasL(R,C), hasR(R,C), not hasU(R,C), not hasD(R,C).
pipe(R,C,"┌") :- white(R,C), not hasU(R,C), hasD(R,C), not hasL(R,C), hasR(R,C).
pipe(R,C,"┐") :- white(R,C), not hasU(R,C), hasD(R,C), hasL(R,C),    not hasR(R,C).
pipe(R,C,"└") :- white(R,C), hasU(R,C),    not hasD(R,C), not hasL(R,C), hasR(R,C).
pipe(R,C,"┘") :- white(R,C), hasU(R,C),    not hasD(R,C), hasL(R,C),    not hasR(R,C).

grid(R,C,"#") :- black(R,C).
grid(R,C,S)   :- pipe(R,C,S).

#show grid/3.
