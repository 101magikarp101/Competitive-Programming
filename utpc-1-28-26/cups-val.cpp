#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 100'000, "N");
    inf.readSpace();
    int c = inf.readInt(1, 100'000, "C");
    inf.readSpace();
    int h = inf.readInt(1, 100'000, "H");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        int a = inf.readInt(1, 1'000'000'000, "A[i]");
        if (i < n - 1) {
            inf.readSpace();
        } else {
            inf.readEoln();
        }
    }
    inf.readEof();
}