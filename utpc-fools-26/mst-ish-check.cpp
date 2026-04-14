#include <bits/stdc++.h>
#include "testlib.h"
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);
    int pans = ouf.readInt();
    int jans = ans.readInt();
    if (pans == jans) {
        quitf(_ok, "u did it");
    } else {
        quitf(_wa, "[loud incorrect buzzer]");
    }
}