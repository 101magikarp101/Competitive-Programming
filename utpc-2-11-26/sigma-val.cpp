#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 100'000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 100'000, "k");
    inf.readEoln();
    int prev = -1;
    for (int i = 0; i < n; i++) {
        int ci = inf.readInt(1, 100'000, "c_i");
        if (i < n - 1) inf.readSpace();
        else inf.readEoln();
        ensuref(ci >= prev, "c_i must be non-decreasing");
    }
    inf.readEof();
}