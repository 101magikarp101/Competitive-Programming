#include <bits/stdc++.h>
using namespace std;

static int n, t, b;
static unordered_map<long long, char> memo;

static long long keyXY(int x, int y) {
    return (long long)x * 2048LL + y; // n<=1000 safe
}

static char ask(int x, int y) {
    long long k = keyXY(x, y);
    auto it = memo.find(k);
    if (it != memo.end()) return it->second;

    cout << "? " << x << " " << y << "\n";
    cout.flush();

    string s;
    if (!(cin >> s)) exit(0);
    if (s == "-1") exit(0);

    char c = s[0]; // 'I', 'P', 'X'
    memo[k] = c;
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> t >> b;

    if (n == 1) {
        cout << "! 0\n";
        cout.flush();
        return 0;
    }

    long long K = 0;     // inside-or-boundary lattice points
    long long Bcnt = 0;  // boundary lattice points

    bool havePrev = false;
    int curLo = t, curHi = t; // start at known boundary point (1,t)

    vector<int> lo(n + 1, 1), hi(n + 1, 0);
    vector<int> len(n + 1, 0);

    for (int x = 1; x <= n; x++) {
        // Compute y(x) on segment (1,t)-(n,b) exactly with rationals.
        long long den = (n - 1);
        long long num = 1LL * t * den + 1LL * (b - t) * (x - 1);

        int yf = (int)(num / den);                 // floor
        int yc = (int)((num + den - 1) / den);     // ceil
        yf = max(1, min(n, yf));
        yc = max(1, min(n, yc));

        int seed = -1;
        if (ask(x, yf) != 'X') seed = yf;
        else if (ask(x, yc) != 'X') seed = yc;

        if (seed == -1) {
            // No lattice points in this column.
            lo[x] = 1; hi[x] = 0; len[x] = 0;
            continue;
        }

        if (!havePrev) {
            curLo = curHi = seed;
            havePrev = true;
        } else {
            curLo = min(curLo, seed);
            curHi = max(curHi, seed);
        }

        // Move curLo up until inside/on (it will stop by seed)
        while (curLo <= n && ask(x, curLo) == 'X') curLo++;
        // Expand downward to minimal
        while (curLo > 1 && ask(x, curLo - 1) != 'X') curLo--;

        // Move curHi down until inside/on
        while (curHi >= 1 && ask(x, curHi) == 'X') curHi--;
        // Expand upward to maximal
        while (curHi < n && ask(x, curHi + 1) != 'X') curHi++;

        lo[x] = curLo;
        hi[x] = curHi;
        len[x] = hi[x] - lo[x] + 1;

        K += len[x];

        // Boundary count:
        if (x == 1 || x == n) {
            // extreme columns: all points here are boundary
            Bcnt += len[x];
        } else {
            if (ask(x, lo[x]) == 'P') Bcnt++;
            if (hi[x] != lo[x] && ask(x, hi[x]) == 'P') Bcnt++;
        }
    }

    long long area2 = 2LL * K - Bcnt - 2; // Pick: 2A = 2K - B - 2
    if (area2 < 0) area2 = 0;

    cout << "! " << area2 << "\n";
    cout.flush();
    return 0;
}
