#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    int t = inf.readInt(1, 1'000'000'000, "t");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        inf.readInt(1, 1'000'000'000, "a");
        if (i < n - 1) inf.readSpace();
    }
    inf.readEoln();
    inf.readEof();
    return 0;
}