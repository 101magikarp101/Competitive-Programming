from itertools import permutations

pieces = [
    {
        "name": "freedom",
        "ans": ["speech"],
        "left": "0 1 2 7 8 -2 6 -0 -5 9",
        "right": "0 1 2 -2 -4 -3 6 3 4 9"
    },
    {
        "name": "bread",
        "ans": ["sustenance", "essentials", "livelihood"],
        "left": "-8 -7 3 4 -3 1 -0 7 8 6",
        "right": "-5 5 -9 2 3 -3 9 7 8 -6"
    },
    {
        "name": "attempt",
        "ans": ["strive", "effort", "hazard", "intend", "hustle", "chance", "aspire", "attack", "pledge", "belief", "tackle", "embark", "plunge", "pursue", "commit", "strain", "scheme"],
        "left": "0 9 3 4 -0 5 -5 7 6 2",
        "right": "0 -4 9 2 3 5 8 7 -1 1"
    },
    {
        "name": "make up one's mind",
        "ans": ["resolve", "appoint", "specify", "espouse"],
        "left": "-5 -0 2 8 5 9 6 -1 7 1",
        "right": "-3 9 2 -3 -4 4 6 8 3 5"
    },
    {
        "name": "director's shout",
        "ans":["action", "places", "lights", "camera"],
        "left": "0 1 -3 3 -1 4 9 -4 6 5",
        "right": "0 1 -5 3 5 9 8 -2 -0 6"
    }, 
    {
        "name": "glance at",
        "ans": ["look", "peek", "scan", "skim", "espy", "ogle"],
        "left": "-0 1 -3 3 -1 6 9 7 8 5",
        "right": "-0 1 -1 3 -2 9 5 7 8 6"
    },
    {
        "name": "conscientiousness",
        "ans":["dutifulness", "carefulness", "application", "uprightness", "heedfulness", "earnestness", "ethicalness"],
        "left": "0 1 2 -3 -2 9 6 5 8 -5",
        "right": "0 1 2 -1 -10 7 6 -4 8 5"
    }
]

# -1 = head, >=0 = piece; also return idx
def go_left(i, j, pieces):
    if i == -1:
        return i, j
    if i == len(pieces):
        return i, j
    res = pieces[i]["right"].split(" ")[j]
    if res[0] == "-":
        return i, (int)(res[1:])
    return go_left(i-1, (int)(res), pieces)

def go_right(i, j, pieces):
    if i == -1:
        return i, j
    if i == len(pieces):
        return i, j
    res = pieces[i]["left"].split(" ")[j]
    if res[0] == "-":
        return i, (int)(res[1:])
    return go_right(i+1, (int)(res), pieces)

def brute(pieces):
    head = ['.'] * 10
    tail = ['.'] * 10
    for i, piece in enumerate(pieces):
        left = piece["left"].split()
        right = piece["right"].split()
        for j, letter in enumerate(piece["ans"]):
            l = -1
            r = -1
            for k in range(10):
                if left[k] == "-"+str(j):
                    l = k
                if right[k] == "-"+str(j):
                    r = k
            # print(piece["ans"], j, l, r)
            # print(left, right)
            if l != -1:
                res = go_left(i-1, l, pieces)
                if res[0] == -1:
                    head[res[1]] = letter
                elif res[0] == len(pieces):
                    tail[res[1]] = letter
                elif pieces[res[0]]["ans"][res[1]] != letter:
                    words = [p["ans"] for p in pieces]
                    print("left", words, piece["ans"], j, pieces[res[0]]["ans"], res[1])
                    return
            if r != -1:
                res = go_right(i+1, r, pieces)
                if res[0] == -1:
                    head[res[1]] = letter
                elif res[0] == len(pieces):
                    tail[res[1]] = letter
                elif pieces[res[0]]["ans"][res[1]] != letter:
                    words = [p["ans"] for p in pieces]
                    # print("right", words, piece["ans"], j, pieces[res[0]]["ans"], res[1])
                    return
    print("<done>")
    print(head)
    print(tail)


def dfs(pieces, i, cur):
    if i == len(pieces):
        brute(cur)
        return

    tmp = pieces[i]
    for j, word in enumerate(tmp["ans"]):
        piece = {
            "name": tmp["name"],
            "ans": word,
            "left": tmp["left"],
            "right": tmp["right"]
        }
        dfs(pieces, i + 1, cur + [piece])
    return

if __name__ == "__main__":
    order = [i for i in range(len(pieces))]
    # brute force all possible orders
    for perm in permutations(order):
        _pieces = [pieces[i] for i in perm]
        dfs(_pieces, 0, [])