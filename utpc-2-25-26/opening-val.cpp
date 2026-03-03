#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 1000, "n");
    inf.readSpace();
    string rc1 = inf.readToken("[A-Z][0-9]+", "s");
    inf.readSpace();
    string rc2 = inf.readToken("[A-Z][0-9]+", "t");
    inf.readEoln();
    inf.readEof();

    string c1 = rc1.substr(1);
    string c2 = rc2.substr(1);

    int num1 = stoi(c1);
    int num2 = stoi(c2);

    // no leading zeros
    ensure(c1[0] != '0' || num1 == 0);
    ensure(c2[0] != '0');
    
    ensure(num1 >= 0 && num1 <= n+1);
    ensure(num2 >= 1 && num2 <= n);
}