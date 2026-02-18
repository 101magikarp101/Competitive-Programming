#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);    

    int N = inf.readInt(1, 200000, "N");
    inf.readSpace();
    int Q = inf.readInt(1, 200000, "Q");
    inf.readEoln();

    string S = inf.readToken("[01]{1,200000}", "S");
    ensuref((int)S.size() == N, "S length must equal N");
    inf.readEoln();

    for (int qi = 1; qi <= Q; qi++) {
        int type = inf.readInt(1, 2, "type");
        inf.readSpace();
        if (type == 1) {
            int i = inf.readInt(1, N, "i");
            inf.readEoln();
            (void)i;
        } else {
            int l = inf.readInt(1, N, "l");
            inf.readSpace();
            int r = inf.readInt(l, N, "r");
            inf.readSpace();
            int k = inf.readInt(1, r - l + 1, "k");
            inf.readEoln();
            (void)k;
        }
    }

    inf.readEof();
    return 0;
}