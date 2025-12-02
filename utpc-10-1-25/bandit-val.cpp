#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector
/*
The first line of input contains $n$, $k$, and $m$ ($1\le n\le m,k\le 10^6$) --- the number of layers of bridges, the number of bridges, and the length of the playing field, respectively.

The next line of input contains $n$ numbers, $l_1, l_2, \ldots ,l_n$ ($1\le l_1< l_2 <\cdots < l_n \le m$) --- the $y$-levels of the bridge layers.

The $j^\text{th}$ of the net $k$ lines contains $3$ space-separated integers, $x_j$, $y_j$, and $d_j$ ($-10^6\le x\le10^6, 1\le y_j\le n, 1\le d_j\le10^6$) --- the $x$-coordinate of the $j^\text{th}$ bridge, the index of the bridge layer it belongs to, and the \textit{debuff} of the bridge, respectively.*/
using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 1'000'000, "n");
    inf.readSpace();
    int k = inf.readInt(1, 1'000'000, "k");
    inf.readSpace();
    int m = inf.readInt(1, 1'000'000, "m");
    inf.readEoln();
    ensuref(n <= m, "n <= m");
    ensuref(n <= k, "n <= k");
    vt<int> l(n);
    set<pair<int,int>> bridges;
    for (int i = 0; i < n; i++) {
        l[i] = inf.readInt(1, m, "l[i]");
        if (i + 1 < n) {
            inf.readSpace();
        } else {
            inf.readEoln();
        }
    }
    for (int i = 0; i < n-1; i++) {
        ensuref(l[i] < l[i+1], "l is strictly increasing");
    }
    for (int i = 0; i < k; i++) {
        int x = inf.readInt(-1'000'000, 1'000'000, "x");
        inf.readSpace();
        int y = inf.readInt(1, n, "y");
        inf.readSpace();
        int d = inf.readInt(1, 1'000'000, "d");
        bridges.insert({x, y});
        inf.readEoln();
    }
    ensuref((int)bridges.size() == k, "all bridges are distinct");
    inf.readEof();
}