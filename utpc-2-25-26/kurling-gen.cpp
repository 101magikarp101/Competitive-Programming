#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Pt { long long x,y; };

static long long cross2(const Pt& a, const Pt& b, const Pt& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static vector<int> genNonincreasingDeltas(int len, int S, int step) {
    vector<int> d(len + 1, 0);
    d[1] = rnd.next(-S, S);
    for (int i = 2; i <= len; i++) d[i] = d[i-1] - rnd.next(0, step);
    return d;
}
static vector<int> genNondecreasingDeltas(int len, int S, int step) {
    vector<int> d(len + 1, 0);
    d[1] = rnd.next(-S, S);
    for (int i = 2; i <= len; i++) d[i] = d[i-1] + rnd.next(0, step);
    return d;
}

static bool buildFromDeltas(int n, const vector<int>& d, vector<int>& arr, int& startLo, int& startHi) {
    // arr[x] = y0 + pref[x], pref[1]=0
    vector<int> pref(n + 1, 0);
    int mn = 0, mx = 0;
    for (int x = 1; x <= n-1; x++) {
        pref[x+1] = pref[x] + d[x];
        mn = min(mn, pref[x+1]);
        mx = max(mx, pref[x+1]);
    }
    startLo = 1 - mn;
    startHi = n - mx;
    if (startLo > startHi) return false;

    int y0 = rnd.next(startLo, startHi);
    arr.assign(n + 1, 0);
    for (int x = 1; x <= n; x++) arr[x] = y0 + pref[x];
    return true;
}

static vector<Pt> compressCollinear(const vector<Pt>& v) {
    vector<Pt> res;
    for (auto p : v) {
        while (res.size() >= 2) {
            Pt a = res[res.size()-2], b = res.back(), c = p;
            if (cross2(a,b,c) == 0) res.pop_back();
            else break;
        }
        res.push_back(p);
    }
    // one more pass for wrap-around collinearity
    bool changed = true;
    while (changed && res.size() >= 3) {
        changed = false;
        int m = (int)res.size();
        for (int i = 0; i < m; i++) {
            Pt a = res[(i-1+m)%m], b = res[i], c = res[(i+1)%m];
            if (cross2(a,b,c) == 0) {
                res.erase(res.begin() + i);
                changed = true;
                break;
            }
        }
    }
    // remove possible duplicate last/first (shouldn't happen, but be safe)
    if (!res.empty() && res.front().x == res.back().x && res.front().y == res.back().y) res.pop_back();
    return res;
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    int n = opt<int>(1);
    int seed = opt<int>(2);
    int type = opt<int>(3);
    rnd.setSeed(seed);

    // type controls roughness
    int S = (type == 1 ? 2 : type == 2 ? 5 : 10);
    int step = (type == 1 ? 1 : type == 2 ? 2 : 3);

    vector<int> U, L;
    int t = -1, b = -1;

    for (int it = 0; it < 20000; it++) {
        auto dU = genNonincreasingDeltas(n-1, S, step);
        auto dL = genNondecreasingDeltas(n-1, S, step);

        vector<int> tmpU, tmpL;
        int loU, hiU, loL, hiL;
        if (!buildFromDeltas(n, dU, tmpU, loU, hiU)) continue;
        if (!buildFromDeltas(n, dL, tmpL, loL, hiL)) continue;

        // Ensure strict separation U[x] > L[x] for all x (try vertical shifting L down if possible)
        int minGap = INT_MAX;
        for (int x = 1; x <= n; x++) minGap = min(minGap, tmpU[x] - tmpL[x]);

        if (minGap <= 0) {
            // Try shifting L down by delta so that minGap becomes 1
            int need = 1 - minGap; // shift L down by need (i.e., L' = L - need)
            int minL = *min_element(tmpL.begin()+1, tmpL.end());
            if (minL - need < 1) continue;
            for (int x = 1; x <= n; x++) tmpL[x] -= need;
        }

        // Now check bounds & strict separation
        bool ok = true;
        for (int x = 1; x <= n; x++) {
            if (!(1 <= tmpU[x] && tmpU[x] <= n)) ok = false;
            if (!(1 <= tmpL[x] && tmpL[x] <= n)) ok = false;
            if (!(tmpU[x] > tmpL[x])) ok = false;
        }
        if (!ok) continue;

        U = tmpU;
        L = tmpL;
        t = U[1];
        b = L[n];
        break;
    }

    if (U.empty()) {
        // Fallback rectangle-ish shape (still strictly convex after compression will be a rectangle)
        U.assign(n+1, n);
        L.assign(n+1, 1);
        t = U[1];
        b = L[n];
    }

    vector<Pt> v;
    v.reserve(2*n + 5);
    for (int x = 1; x <= n; x++) v.push_back({x, U[x]});
    for (int x = n; x >= 1; x--) v.push_back({x, L[x]});

    // remove consecutive duplicates
    vector<Pt> vv;
    for (auto p : v) if (vv.empty() || vv.back().x != p.x || vv.back().y != p.y) vv.push_back(p);
    if (!vv.empty() && vv.front().x == vv.back().x && vv.front().y == vv.back().y) vv.pop_back();

    vv = compressCollinear(vv);

    // Ensure CCW orientation
    long long signedA = 0;
    for (int i = 0; i < (int)vv.size(); i++) {
        Pt a = vv[i], b2 = vv[(i+1)%vv.size()];
        signedA += a.x * b2.y - a.y * b2.x;
    }
    if (signedA < 0) reverse(vv.begin(), vv.end());

    cout << n << " " << t << " " << b << "\n";
    cout << (int)vv.size() << "\n";
    for (auto p : vv) cout << p.x << " " << p.y << "\n";
    return 0;
}
