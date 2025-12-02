#!/usr/bin/env python3
# Brute‑force all possible round‑robin results for 6 players (0 through 5).

# judges_strict = [
#     [-1],
#     [0],
#     [-1],
#     [-1],
#     [-1],
#     [-1],
#     [-1],
#     [-1],
#     [1,2,5],
#     [-1]
# ]

judges_strict = {
    1: {2: [0,1], 3: [2,4,5], 4: [2,3,4]},
    2: {5: [0,2,3], 8: [0,1,2,3]},
    3: {2: [0], 4: [1,3,4]},
    4: {2: [0], 9: [1,2,5]},
    5: {2: [1,3], 9: [0,1,3,4], 10: [0,4]},
    6: {3: [1,2,4,5], 5: [1,2,3], 7: []},
    7: {},
    8: {},
    9: {},
    10: {}
}

judges_include = {
    1: {},
    2: {},
    3: {5: [3,5], 8: [2], 10: [4]},
    4: {1: [5], 7: [2]},
    5: {1: [4]},
    6: {},
    7: {9: [1]},
    8: {},
    9: {},
    10: {}
}

# Beat team 2 (1-indexed)
def j1(grid):
    return [i for i in range(6) if grid[i][1] == 1]

# Most wins
def j2(grid):
    wins = [sum(row) for row in grid]
    max_wins = max(wins)
    return [i for i in range(6) if wins[i] == max_wins]

# Beat higher seed
def j3(grid):
    return [i for i in range(6) if any(grid[i][j] == 1 for j in range(0, i))]

# Count wins per team (w_i), find frequency array of w_i (freq), return i such that w_i = max(freq)
def j4(grid):
    wa = [row.count(1) for row in grid]
    ma = [wa.count(wins) for wins in wa]
    mc = max(ma)
    return [player for player, freq in zip(range(6), ma) if freq == mc]

# Beat team (i+1)%6
def j5(grid):
    return [i for i in range(6) if grid[i][(i + 1) % 6] == 1]

# No upsets
def j6(grid):
    return [i for i in range(6) if all(grid[i][j] == 1 for j in range(i+1, 6))]

# w_i = w_{(i+1)%6}
def j7(grid):
    wins = [sum(row) for row in grid]
    return [i for i in range(6) if wins[i] == wins[(i + 1) % 6]]

# // Top cycle.
# function TC_J() {
# 	var out = [];
# 	for (var r = 0; r < NUM_PLAYERS; r++) {
# 		var vis = 1 << r;
# 		var s = [r];
# 		while (s.length) {
# 			var u = s.pop();
# 			for (var v = 0; v < NUM_PLAYERS; v++) {
# 				if (tourState[u][v] == 1 && (vis & (1 << v)) == 0) {
# 					vis |= (1 << v);
# 					s.push(v);
# 				}
# 			}
# 		}
# 		if (vis == (1 << NUM_PLAYERS) - 1) {
# 			out.push(r);
# 		}
# 	}
# 	return out;
# }
def j8(grid):
    out = []
    for r in range(6):
        vis = 1 << r
        s = [r]
        while s:
            u = s.pop()
            for v in range(6):
                if grid[u][v] == 1 and (vis & (1 << v)) == 0:
                    vis |= (1 << v)
                    s.append(v)
        if vis == (1 << 6) - 1:
            out.append(r)
    return out

# Revengers, win against team that beat them in prev
def j9(grid):
    if len(TOURS) < 2:
        return []
    return [i for i in range(6) if any(grid[i][j] == 1 and TOURS[-1][i][j] == 0 for j in range(6))]

# Seed divides #wins.
def j10(grid):
    nwins = [sum(row) for row in grid]
    nwins = [nwins[i] + WINS[i] for i in range(6)]
    return [i for i in range(6) if nwins[i] % (i + 1) == 0 and nwins[i] > 0]

def run_judge(judge, grid):
    if judge == 1:
        return j1(grid)
    elif judge == 2:
        return j2(grid)
    elif judge == 3:
        return j3(grid)
    elif judge == 4:
        return j4(grid)
    elif judge == 5:
        return j5(grid)
    elif judge == 6:
        return j6(grid)
    elif judge == 7:
        return j7(grid)
    elif judge == 8:
        return j8(grid)
    elif judge == 9:
        return j9(grid)
    elif judge == 10:
        return j10(grid)

def check(grid, tour_id):
    """Check the grid against the judges' criteria."""
    ress = [[]]
    for i in range(1, 11):
        res = run_judge(i, grid)
        if i in judges_strict[tour_id] and judges_strict[tour_id][i] != res:
            return False
        if i in judges_include[tour_id] and not set(judges_include[tour_id][i]).issubset(set(res)):
            return False
        ress.append(res)
    if tour_id == 5:
        if len(ress[8]) != 4:
            return False
    elif tour_id == 6:
        if 4 in ress[9]:
            return False
    elif tour_id == 7:
        if len(ress[3]) != 1:
            return False
        elif len(ress[6]) != 5:
            return False
    elif tour_id == 8:
        if len(ress[2]) >= 3:
            return False
        elif len(ress[3]) >= 3:
            return False
        elif len(ress[6]) <= 3:
            return False
        elif len(ress[7]) <= 3:
            return False
        elif len(ress[10]) <= 3:
            return False
    elif tour_id == 9:
        if ress[1] != ress[2]:
            return False
        elif len(ress[2]) != 2:
            return False
        elif len(ress[3]) != 2:
            return False
        elif set(ress[4]+ress[9]) != set([0,1,2,3,4,5]):
            return False
        elif not set(ress[6]).issubset(set(ress[5])) or len(ress[6]) >= len(ress[5]):
            return False
        elif not set(ress[3]).issubset(set(ress[6])) or len(ress[3]) >= len(ress[6]):
            return False
        elif len(ress[7]) < 1:
            return False
        elif len(ress[9]) != 3:
            return False
        elif len(ress[10]) != 3:
            return False
    elif tour_id == 10:
        # Judge 1: If we ignore Team 5 and all its matches, exactly 2 teams would win.
        winners = [i for i in range(6) if i != 4 and grid[i][1] == 1]
        if len(winners) != 2:
            return False
        # Judge 2: If we ignore Team 2 and all its matches, every team would win.
        wins = [0]*6
        for i in range(6):
            if i == 1:
                continue
            for j in range(6):
                if grid[i][j] == 1 and j != 1:
                    wins[i] += 1
        max_wins = max(wins)
        winners = [i for i in range(6) if wins[i] == max_wins]
        if len(winners) != 5:
            return False
        # # Judge 3: If we ignore Team 4 and all its matches, only teams 3 and 6 would win.
        winners = [i for i in range(6) if i != 3 and any(grid[i][j] == 1 for j in range(0, i) if j != 3)]
        if winners != [2, 5]:
            return False
        # # Judge 6: If we ignore Team 6 and all its matches, only teams 3 and 5 would win.
        winners = [i for i in range(6) if i != 5 and all(grid[i][j] == 1 for j in range(i+1, 6) if j != 5)]
        if winners != [2, 4]:
            return False
    return True

def brute_round_robin(tour_id):
    global TOURS, WINS
    print(f"Checking tournament {tour_id}...")
    if tour_id > 10:
        real_tours = TOURS[:]
        real_wins = WINS[:]
        #print all solutions
        for i in range(len(real_tours)):
            print(f"Tournament {i+1}")
            for row in real_tours[i]:
                print(''.join(str(x) for x in row))
            TOURS = real_tours[0:i]
            WINS = [0]*6
            for j in range(i):
                WINS = [WINS[k] + sum(real_tours[j][k]) for k in range(6)]
            res = run_judge(i+1, real_tours[i])
            print(' '.join(str(x+1) for x in res))
        exit(0)
    matches = [(i, j) for i in range(6) for j in range(i + 1, 6)]
    total = 1 << len(matches)
    for mask in range(total):
        grid = [[0] * 6 for _ in range(6)]
        for bit_idx, (i, j) in enumerate(matches):
            if (mask >> bit_idx) & 1:
                grid[i][j] = 1
            else:
                grid[j][i] = 1
        if check(grid, tour_id):
            TOURS.append(grid)
            WINS = [WINS[i] + sum(grid[i]) for i in range(6)]
            brute_round_robin(tour_id + 1)
            WINS = [WINS[i] - sum(grid[i]) for i in range(6)]
            TOURS.pop()

if __name__ == "__main__":
    TOURS = []
    WINS = [0, 0, 0, 0, 0, 0]
    brute_round_robin(1)
