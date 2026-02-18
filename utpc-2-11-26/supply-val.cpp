#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector
#define ll long long

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 200'000, "m");
    inf.readSpace();
    int q = inf.readInt(1, 200'000, "q");
    inf.readEoln();

    for (int i = 0; i < n; i++) {
        int ai = inf.readInt(0, 1'000'000, "a_i");
        inf.readSpace();
        int bi = inf.readInt(0, 1'000'000, "b_i");
        inf.readSpace();
        int ci = inf.readInt(0, 1'000'000, "c_i");
        inf.readEoln();
        ensuref((ll) ai - (ll) bi * m >= 0, "a_i - b_i * m must be non-negative");
    }

    for (int i = 0; i < q; i++) {
        int li = inf.readInt(1, n, "l_i");
        inf.readSpace();
        int ri = inf.readInt(li, n, "r_i");
        inf.readEoln();
    }
    inf.readEof();
}