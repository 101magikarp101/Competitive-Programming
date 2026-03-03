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
        int l = inf.readInt(1, n, "l");
        inf.readSpace();
        int r = inf.readInt(1, n, "r");
        inf.readEoln();
        ensuref(l <= r, "l must be less than or equal to r");
    }
    inf.readEof();
}