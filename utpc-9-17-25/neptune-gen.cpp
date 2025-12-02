#include "testlib.h"
#include <iostream>
#define vt vector
#define ll long long

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int mink = opt<int>(3);
    int maxk = opt<int>(4);
    ll range = opt<ll>(5);
    int seed = opt<int>(6);
    rnd.setSeed(seed);
    int N = rnd.next(minn, maxn);
    int K = rnd.next(mink, maxk);
    int p = rnd.next(1, 100);
    int p2 = rnd.next(1, 30);
    vt<vt<ll>> v(1, vt<ll>(K));
    for (int i = 0; i < K; i++) {
        v[0][i] = rnd.next(0LL, range);
    }
    cout << N << ' ' << K << '\n';
    for (int i = 0; i < N; i++) {
        int x = rnd.next(1, 100);
        int y = rnd.next(0, (int)v.size()-1);
        v.push_back(vt<ll>(K));
        if (x <= p) {
            for (int j = 0; j < K; j++) {
                if (rnd.next(1, 100) <= p2) {
                    v.back()[j] = 0;
                } else {
                    v.back()[j] = v[y][j];
                }
            }
            for (int j = 0; j < K; j++) {
                cout << v.back()[j];
                if (j < K - 1) cout << ' ';
                else cout << '\n';
            }
        } else {
            for (int j = 0; j < K; j++) {
                v.back()[j] = rnd.next(0LL, range);
            }
            for (int j = 0; j < K; j++) {
                cout << v.back()[j];
                if (j < K - 1) cout << ' ';
                else cout << '\n';
            }
        }
    }
}