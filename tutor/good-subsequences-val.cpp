#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    registerValidation();

    int N = inf.readInt(1, 200000, "N");
    inf.readSpace();
    int K = inf.readInt(1, 20, "K");
    inf.readEoln();

    for (int i = 1; i <= N; i++) {
        int ai = inf.readInt(1, N, ("a[" + to_string(i) + "]").c_str());
        if (i < N) inf.readSpace();
        else inf.readEoln();
        (void)ai;
    }

    inf.readEof();
    return 0;
}