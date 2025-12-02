#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int minw = opt<int>(3);
    int maxw = opt<int>(4);
    int minh = opt<int>(5);
    int maxh = opt<int>(6);

    int minx = opt<int>(7);
    int maxx = opt<int>(8);
    int seed = opt<int>(9);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    cout << n << endl;
    for (int i = 0; i < n; i++) {
        int w = rnd.next(minw, maxw);
        int h = rnd.next(minh, maxh);
        cout << w << " " << h << " ";
        int x1 = rnd.next(minx, maxx);
        int y1 = rnd.next(minx, maxx);
        while (x1 == 0 && y1 == 0) {
            x1 = rnd.next(minx, maxx);
            y1 = rnd.next(minx, maxx);
        }
        cout << x1 << " " << y1 << " ";
        int x2 = rnd.next(minx, maxx);
        int y2 = rnd.next(minx, maxx);
        while (x2 == 0 && y2 == 0) {
            x2 = rnd.next(minx, maxx);
            y2 = rnd.next(minx, maxx);
        }
        cout << x2 << " " << y2 << endl;
    }
}