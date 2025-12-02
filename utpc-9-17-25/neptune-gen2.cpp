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
    int N = rnd.next(1, n);
    int K = rnd.next(1, k);
    cout << N << ' ' << K << '\n';
    set<ll> s;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            ll x = rnd.next(0LL, range);
            while (s.count(x)) {
                x = rnd.next(0LL, range);
            }
            s.insert(x);
            cout << x;
            if (j < K - 1) cout << ' ';
            else cout << '\n';
        }
    }
}