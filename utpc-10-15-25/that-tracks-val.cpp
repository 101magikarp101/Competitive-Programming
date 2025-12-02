#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 200'000, "n");
    inf.readEoln();
    string s[2];
    s[0] = inf.readLine("[V>^<.]{"+to_string(2*n)+"}", "s0");
    s[1] = inf.readLine("[V>^<.]{"+to_string(2*n)+"}", "s1");
    inf.readEof();
    for (int i = 0; i < 2*n; i+=2) {
        ensuref(s[0][i] != '.', "Top at column %d", i);
        bool b = s[1][i] != '.';
        bool r = s[0][i+1] != '.';
        if (b && r) {
            ensuref(false, "Both bottom and right at column %d", i);
        }
        ensuref(s[1][i+1] == '.', "Bottom at column %d", i);
    }
    return 0;
}