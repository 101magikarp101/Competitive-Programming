#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 16, "n");
    inf.readSpace();
    int k = inf.readInt(1, 1'000'000'000, "k");
    inf.readEoln();
    for (int i = 0; i < n; i++) {
        int a = inf.readInt(1, 1'000'000'000, "a_i");
        if (i < n - 1) inf.readSpace();
        else inf.readEoln();
    }
    inf.readEof();
}