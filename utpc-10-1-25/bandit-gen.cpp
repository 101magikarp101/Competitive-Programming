#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int mink = opt<int>(3);
    int maxk = opt<int>(4);
    int minm = opt<int>(5);
    int maxm = opt<int>(6);

    int minx = opt<int>(7);
    int maxx = opt<int>(8);
    int mind = opt<int>(9);
    int maxd = opt<int>(10);
    int seed = opt<int>(11);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    int k = rnd.next(max(n, mink), maxk);
    int m = rnd.next(max(n, minm), maxm);
    cout << n << " " << k << " " << m << endl;
    vt<int> v;
    for (int i = 1; i <= m; i++) v.push_back(i);
    shuffle(v.begin(), v.end());
    vt<int> l(v.begin(), v.begin()+n);
    sort(l.begin(), l.end());
    for (int i = 0; i < n; i++) {
        if (i) cout << " ";
        cout << l[i];
    }
    cout << endl;

    set<pair<int,int>> bridges;
    set<int> ys;
    v = vt<int>();
    for (int i = 1; i <= n; i++) v.push_back(i);
    shuffle(v.begin(), v.end());
    while ((int)ys.size() < n) {
        int x = rnd.next(minx, maxx);
        int y = v.back();
        int d = rnd.next(mind, maxd);
        if (!bridges.count({x,y})) {
            bridges.insert({x,y});
            ys.insert(y);
            v.pop_back();
            cout << x << " " << y << " " << d << endl;
        }
    }
    while ((int)bridges.size() < k) {
        int x = rnd.next(minx, maxx);
        int y = rnd.next(1, n);
        int d = rnd.next(mind, maxd);
        if (!bridges.count({x,y})) {
            bridges.insert({x,y});
            cout << x << " " << y << " " << d << endl;
        }
    }
}