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
    int mina = opt<int>(5);
    int maxa = opt<int>(6);
    int seed = opt<int>(7);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    int k = rnd.next(mink, min(n, maxk));
    cout << n << " " << k << endl;
    for (int i = 0; i < n; i++) {
        cout << rnd.next(mina, maxa);
        if (i < n - 1) cout << " ";
    }
    cout << endl;
    return 0;
}