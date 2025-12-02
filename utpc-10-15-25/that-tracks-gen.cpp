#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

string cs = "V>^<";

vt<vt<char>> get(int x) {
    vt<vt<char>> res(2, vt<char>(2, '.'));
    if (x < 4) {
        res[0][0] = cs[x];
    } else if (x < 20) {
        x -= 4;
        res[1][0] = cs[x/4];
        res[0][0] = cs[x%4];
    } else {
        x -= 20;
        res[0][0] = cs[x/4];
        res[0][1] = cs[x%4];
    }
    return res;
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int seed = opt<int>(3);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    cout << n << "\n";
    vt<vt<char>> a(2,vt<char>(2*n));
    for (int i = 0; i < 2*n; i+=2) {
        int x = rnd.next(0,35);
        vt<vt<char>> b = get(x);
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                a[j][i+k] = b[j][k];
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2*n; j++) {
            cout << a[i][j];
        }
        cout << "\n";
    }
    return 0;
}