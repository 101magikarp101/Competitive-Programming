#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    inf.readSpace();
    int k = inf.readInt(1, 200'000, "k");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        int x = inf.readInt(-1000, 1000, "x");
        if (i < n - 1) inf.readSpace();
        else inf.readEoln();
    }
    for (int i = 0; i < k; i++) {
        int l = inf.readInt(1, n, "l");
        inf.readSpace();
        int r = inf.readInt(l, n, "r");
        inf.readEoln();
    }
    inf.readEof();
}