#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 200'000, "m");
    inf.readEoln();
    for (int i = 0; i < m; i++) {
        int u = inf.readInt(1, n, "u");
        inf.readSpace();
        int v = inf.readInt(1, n, "v");
        inf.readSpace();
        int w = inf.readInt(1, 1'000'000'000, "w");
        inf.readEoln();
        ensuref(1 <= u && u <= n, "u=%d is out of range", u);
        ensuref(1 <= v && v <= n, "v=%d is out of range", v);
    }
    inf.readEof();
}