#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        int w = inf.readInt(1, 1'000'000'000, "w");
        inf.readSpace();
        int h = inf.readInt(1, 1'000'000'000, "h");
        inf.readSpace();
        int x1 = inf.readInt(0, 1'000'000'000, "x1");
        inf.readSpace();
        int y1 = inf.readInt(0, 1'000'000'000, "y1");
        inf.readSpace();
        ensure(x1 != 0 || y1 != 0);
        int x2 = inf.readInt(0, 1'000'000'000, "x2");
        inf.readSpace();
        int y2 = inf.readInt(0, 1'000'000'000, "y2");
        inf.readEoln();
        ensure(x2 != 0 || y2 != 0);
    }
    inf.readEof();
    return 0;
}