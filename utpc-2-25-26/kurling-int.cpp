#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Pt {
    long long x, y;
};

static long long cross2(const Pt& a, const Pt& b, const Pt& c) {
    // cross((b-a),(c-a))
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegment(const Pt& a, const Pt& b, const Pt& p) {
    if (cross2(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

static long long area2_abs(const vector<Pt>& poly) {
    long long s = 0;
    int m = (int)poly.size();
    for (int i = 0; i < m; i++) {
        const Pt& a = poly[i];
        const Pt& b = poly[(i + 1) % m];
        s += a.x * b.y - a.y * b.x;
    }
    return llabs(s);
}

static char classifyPoint(const vector<Pt>& poly, const Pt& p) {
    int m = (int)poly.size();
    // Perimeter check first
    for (int i = 0; i < m; i++) {
        if (onSegment(poly[i], poly[(i + 1) % m], p)) return 'P';
    }
    // Inside check for convex polygon in CCW: all cross >= 0; strict interior means all > 0.
    for (int i = 0; i < m; i++) {
        long long z = cross2(poly[i], poly[(i + 1) % m], p);
        if (z < 0) return 'X';
    }
    return 'I';
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    int n = inf.readInt(2, 1000, "n"); inf.readSpace();
    int t = inf.readInt(1, n, "t"); inf.readSpace();
    int b = inf.readInt(1, n, "b"); inf.readEoln();

    int k = inf.readInt(3, 4000, "k"); inf.readEoln();
    vector<Pt> poly(k);
    for (int i = 0; i < k; i++) {
        poly[i].x = inf.readInt(1, n, "x");
        inf.readSpace();
        poly[i].y = inf.readInt(1, n, "y");
        inf.readEoln();
    }
    inf.readEof();

    // Ensure CCW orientation for inside test. If signed area negative, reverse.
    long long signedA = 0;
    for (int i = 0; i < k; i++) {
        const Pt& a = poly[i];
        const Pt& b2 = poly[(i + 1) % k];
        signedA += a.x * b2.y - a.y * b2.x;
    }
    if (signedA < 0) reverse(poly.begin(), poly.end());

    const long long want = area2_abs(poly);
    const int qlim = 10 * n;
    int used = 0;

    // Send only public parameters to contestant
    cout << n << " " << t << " " << b << endl;

    while (true) {
        if (ouf.seekEof()) {
            quitf(_wa, "contestant terminated without printing answer");
        }
        string cmd = ouf.readToken();
        if (cmd == "?") {
            int x = ouf.readInt(1, n, "x");
            int y = ouf.readInt(1, n, "y");
            used++;
            if (used > qlim) {
                cout << -1 << endl;
                quitf(_wa, "too many queries: %d > %d", used, qlim);
            }
            char res = classifyPoint(poly, Pt{(long long)x, (long long)y});
            cout << res << endl;
        } else if (cmd == "!") {
            long long p = ouf.readLong(0, (long long)4e18, "p");
            ouf.readEof();
            if (p != want) {
                quitf(_wa, "wrong answer: expected %lld got %lld (queries=%d)", want, p, used);
            }
            quitf(_ok, "correct (area2=%lld, queries=%d)", want, used);
        } else {
            cout << -1 << endl;
            quitf(_wa, "unknown command: '%s'", cmd.c_str());
        }
    }
}
