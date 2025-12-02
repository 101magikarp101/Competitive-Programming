#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int n = opt<int>(1);
    int k = opt<int>(2);
    int range = opt<int>(3);
    int seed = opt<int>(4);
    rnd.setSeed(seed);
    cout << rnd.next(1, n) << ' ' << rnd.next(1, k) << '\n';
    for (int i = 0; i < n; i++) {
        cout << rnd.next(-range, range);
        if (i < n - 1) cout << ' ';
        else cout << '\n';
    }
    for (int i = 0; i < k; i++) {
        int l = rnd.next(1, n);
        int r = rnd.next(l, n);
        cout << l << ' ' << r << '\n';
    }
    return 0;
}