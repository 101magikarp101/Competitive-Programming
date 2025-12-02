#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    string s;
    for (int i = 0; i < 1000; i++) {
        s += 'a';
    }
    cout << s << endl;
    cout << 1000 << endl;
    string t;
    for (int i = 0; i < 30; i++) {
        t += 'z';
    }
    for (int i = 0; i < 1000; i++) {
        cout << t << endl;
    }
}