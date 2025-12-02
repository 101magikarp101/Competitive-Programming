#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int seed = opt<int>(3);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    cout << n << endl;
}