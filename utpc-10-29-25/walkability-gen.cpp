#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int minu = opt<int>(3);
    int maxu = opt<int>(4);
    int mina = opt<int>(5);
    int maxa = opt<int>(6);
    int n = rnd.next(minn, maxn);
    cout << n << endl;
    int u = rnd.next(minu, min(maxu, n));
    set<int> s;
    vt<int> a;
    while ((int)s.size() < u) {
        int x = rnd.next(mina, maxa);
        if (s.count(x)) continue;
        s.insert(x);
        a.push_back(x);
    }
    while ((int)a.size() < n) {
        int x = rnd.next(0, u-1);
        a.push_back(a[x]);
    }
    shuffle(a.begin(), a.end());
    for (int i = 0; i < n; i++) {
        cout << a[i];
        if (i < n - 1) cout << " ";
    }
    cout << endl;
    return 0;
}