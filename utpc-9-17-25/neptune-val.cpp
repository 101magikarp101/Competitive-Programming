#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector
#define ll long long

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 1000, "n");
    inf.readSpace();
    int k = inf.readInt(1, 1000, "k");
    inf.readEoln();
    set<ll> s;
    for (int i = 0; i < n; i++) {
        ll hm = 0;
        for (int j = 0; j < k; j++) {
            ll x = inf.readLong(0, 1'000'000'000'000'000'000LL, "x");
            hm = hm*1000'000'000'000'000'003LL + x;
            if (j < k - 1) inf.readSpace();
            else inf.readEoln();
        }
        s.insert(hm);
    }
    // fail if duplicate rows
    ensuref((int)s.size() == n, "duplicate rows");
    inf.readEof();
}