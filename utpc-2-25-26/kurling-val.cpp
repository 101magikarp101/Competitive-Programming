#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Pt { long long x, y; };

static long long cross2(const Pt& a, const Pt& b, const Pt& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegment(const Pt& a, const Pt& b, const Pt& p) {
    if (cross2(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(2, 1000, "n"); inf.readSpace();
    int t = inf.readInt(1, n, "t"); inf.readSpace();
    int b = inf.readInt(1, n, "b"); inf.readEoln();

    int k = inf.readInt(3, 4000, "k"); inf.readEoln();
    vector<Pt> p(k);
    for (int i = 0; i < k; i++) {
        p[i].x = inf.readInt(1, n, "x");
        inf.readSpace();
        p[i].y = inf.readInt(1, n, "y");
        inf.readEoln();
    }
    inf.readEof();

    // Distinct vertices
    {
        set<pair<long long,long long>> st;
        for (int i = 0; i < k; i++) {
            auto pr = make_pair(p[i].x, p[i].y);
            ensuref(!st.count(pr), "duplicate vertex (%lld,%lld)", p[i].x, p[i].y);
            st.insert(pr);
        }
    }

    // Strict convexity: all turns have same sign and non-zero
    int sign = 0;
    for (int i = 0; i < k; i++) {
        Pt a = p[(i - 1 + k) % k], b2 = p[i], c = p[(i + 1) % k];
        long long z = cross2(a, b2, c);
        ensuref(z != 0, "collinear consecutive vertices at i=%d", i);
        int s = (z > 0 ? 1 : -1);
        if (sign == 0) sign = s;
        else ensuref(sign == s, "non-convex or inconsistent orientation at i=%d", i);
    }

    // Promise points on perimeter
    Pt A{1, t}, B{n, b};
    bool okA = false, okB = false;
    for (int i = 0; i < k; i++) {
        Pt u = p[i], v = p[(i + 1) % k];
        if (onSegment(u, v, A)) okA = true;
        if (onSegment(u, v, B)) okB = true;
    }
    ensuref(okA, "(1,t) = (1,%d) is not on perimeter", t);
    ensuref(okB, "(n,b) = (%d,%d) is not on perimeter", n, b);

    return 0;
}
