#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector
#define sz(x) (int)((x).size())
using ll = long long;
using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    ll i = inf.readLong(2, 1'000'000'000'000'000'000LL, "i");
    inf.readSpace();
    string a = inf.readToken("[0-9]{1,100}", "a");
    inf.readEoln();
    int j = inf.readInt(2, 10, "j");
    inf.readEoln();
    inf.readEof();

    // Check that a is a valid base-i number
    for (char c : a) {
        ensuref(c >= '0' && c < '0'+i, "a=%s is not a valid base-%d number", a.c_str(), i);
    }
    // check that a_i <= 1e18
    ll res = 0;
    ll p = 1;
    for (int k = sz(a)-1; k >= 0; k--) {
        res += p*(a[k]-'0');
        p *= i;
        ensuref(res <= 1'000'000'000'000'000'000LL, "a=%s is too large (a_i=%lld is too large)", a.c_str(), res);
    }
}