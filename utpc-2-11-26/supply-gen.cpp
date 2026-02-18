#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static int64 clamp_ll(int64 x, int64 lo, int64 hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1); // command line: gen seed n m q type

    int n = opt<int>(2);
    int m = opt<int>(3);
    int q = opt<int>(4);
    int type = opt<int>(5);

    const int AMAX = 1'000'000;
    const int BMAX = 1'000'000;
    const int CMAX = 1'000'000;

    cout << n << " " << m << " " << q << "\n";

    vector<int> a(n + 1), b(n + 1), c(n + 1);

    for (int i = 1; i <= n; i++) {
        int ai = 0, bi = 0, ci = 0;

        if (type == 6) {
            // MAX ANSWER construction (under constraints + nonnegative-demand guarantee):
            // choose b=0, c=0, a=1e6. Then profit per shop is a*p, max at p=m.
            ai = AMAX;
            bi = 0;
            ci = 0;
        } else if (type == 7) {
            // NEGATIVE construction:
            // b=0, a=1e6, c=1e6. For p in [0,m] with m<=2e5, (p-c)<0 always => profit negative.
            ai = AMAX;
            bi = 0;
            ci = CMAX;
        } else if (type == 5) {
            // HIGH POSITIVE family:
            // make a huge, c tiny, b mostly 0 or very small (still respecting b <= a/m).
            ai = rnd.next(800'000, AMAX);
            ci = rnd.next(0, 20); // tiny cost

            int bcap = (m > 0 ? ai / m : 0);
            bcap = min(bcap, BMAX);

            int coin = rnd.next(0, 99);
            if (bcap == 0) bi = 0;
            else if (coin < 70) bi = 0;                 // lots of linear pieces => best near p=m
            else bi = rnd.next(0, min(bcap, 5));         // tiny slope to exercise quadratic case
        } else if (type == 3) {
            ai = AMAX;
            ci = rnd.next(0, CMAX);
            bi = (m > 0 ? ai / m : 0);
            bi = min(bi, BMAX);
        } else if (type == 4) {
            if (rnd.next(0, 99) < 70) ai = 0;
            else ai = rnd.next(0, AMAX);
            ci = (rnd.next(0, 99) < 50) ? 0 : rnd.next(0, CMAX);

            int bcap = (m > 0 ? ai / m : 0);
            bcap = min(bcap, BMAX);

            if (rnd.next(0, 99) < 60) bi = 0;
            else if (bcap > 0 && rnd.next(0, 99) < 70) bi = bcap;
            else bi = (bcap == 0 ? 0 : rnd.next(0, bcap));
        } else if (type == 1) {
            ai = rnd.next(0, AMAX);
            ci = rnd.next(0, CMAX);
            bi = 0; // linear case
        } else if (type == 2) {
            ai = rnd.next(0, AMAX);
            ci = rnd.next(0, CMAX);

            int bcap = (m > 0 ? ai / m : 0);
            bcap = min(bcap, BMAX);

            if (bcap == 0) bi = 0;
            else if (rnd.next(0, 99) < 70) bi = bcap;   // "tight" often
            else bi = rnd.next(0, bcap);
        } else {
            // type == 0: fully random respecting guarantee
            ai = rnd.next(0, AMAX);
            ci = rnd.next(0, CMAX);

            int bcap = (m > 0 ? ai / m : 0);
            bcap = min(bcap, BMAX);
            bi = (bcap == 0 ? 0 : rnd.next(0, bcap));
        }

        // enforce bounds
        ai = (int)clamp_ll(ai, 0, AMAX);
        bi = (int)clamp_ll(bi, 0, BMAX);
        ci = (int)clamp_ll(ci, 0, CMAX);

        // enforce nonnegative-demand guarantee explicitly: b*m <= a
        if (m > 0) bi = min(bi, ai / m);

        a[i] = ai; b[i] = bi; c[i] = ci;
        cout << a[i] << " " << b[i] << " " << c[i] << "\n";
    }

    // Queries: ensure types 6/7 include a full-range query to witness extreme answer.
    for (int j = 1; j <= q; j++) {
        int l, r;

        if (j == 1) {
            l = 1; r = n;            // always include full range
        } else if (j == 2) {
            l = 1; r = 1;
        } else if (j == 3) {
            l = n; r = n;
        } else {
            int coin = rnd.next(0, 99);
            int len;
            if (coin < 55) len = rnd.next(1, min(n, 100));
            else if (coin < 85) len = rnd.next(1, min(n, 10'000));
            else len = rnd.next(1, n);

            l = rnd.next(1, n - len + 1);
            r = l + len - 1;
        }

        cout << l << " " << r << "\n";
    }

    return 0;
}
