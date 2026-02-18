#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 100'000, "N");
    inf.readEoln();
    set<pair<int,int>> s;
    for (int i = 0; i < n; i++) {
        int u = inf.readInt(-1'000'000'000, 1'000'000'000, "U[i]");
        inf.readSpace();
        int v = inf.readInt(-1'000'000'000, 1'000'000'000, "V[i]");
        inf.readEoln();
        pair<int,int> p = {u,v};
        ensuref(s.find(p) == s.end(), "Point (%d, %d) is duplicated.", u, v);
        s.insert(p);
    }
    inf.readEof();
}