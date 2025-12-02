#include "testlib.h"
#include <iostream>
#define vt vector
#define ll long long

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int n = opt<int>(1);
    int k = opt<int>(2);
    ll range = opt<ll>(3);
    int seed = opt<int>(4);
    rnd.setSeed(seed);
    vt<vt<ll>> v(1, vt<ll>(k));
    for (int i = 0; i < k; i++) {
        v[0][i] = rnd.next(1LL, range);
    }
    int ans = rnd.next(max(n-20, 1), n);
    for (int i = 1; i < ans; i++) {
        vt<ll> row(k);
        for (int j = 0; j < k; j++) {
            row[j] = v.back()[j];
        }
        int x = rnd.next(0, (int)v.size()-1);
        while (row[x] == 0) {
            x = rnd.next(0, (int)v.size()-1);
        }
        row[x] = 0;
        v.push_back(row);
    }
    for (int i = ans; i < n; i++) {
        vt<ll> row(k);
        for (int j = 0; j < k; j++) {
            row[j] = rnd.next(1LL, range);
        }
        v.push_back(row);
    }
    shuffle(v.begin(), v.end());
    cout << n << ' ' << k << '\n';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cout << v[i][j];
            if (j < k - 1) cout << ' ';
            else cout << '\n';
        }
    }
    return 0;
}