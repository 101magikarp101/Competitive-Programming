#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    string s = inf.readToken("[a-z]{1,1000}");
    inf.readEoln();
    int N = inf.readInt(1, 1000);
    inf.readEoln();
    for (int i = 0; i < N; i++) {
        string t = inf.readToken("[a-z]{1,30}");
        inf.readEoln();
    }
    inf.readEof();
    return 0;
}