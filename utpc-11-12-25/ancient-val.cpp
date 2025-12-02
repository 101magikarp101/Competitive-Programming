#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(3, 100, "N");
    inf.readSpace();
    int k = inf.readInt(3, n, "K");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        inf.readInt(1, 10000, "a[i]");
        if (i < n - 1) inf.readSpace();
    }
    inf.readEoln();
    inf.readEof();
    return 0;
}