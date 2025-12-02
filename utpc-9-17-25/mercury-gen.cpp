#include "testlib.h"
#include <iostream>
#define vt vector

using namespace std;

string rot(string &s, int x) {
    int n = (int)s.size();
    string res = s;
    for (int i = 0; i < n; i++) {
        int c = s[i]-'a';
        c = (c+x)%26;
        res[i] = 'a'+c;
    }
    return res;
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int minn = opt<int>(1);
    int maxn = opt<int>(2);
    int minm = opt<int>(3);
    int maxm = opt<int>(4);
    int minl = opt<int>(5);
    int maxl = opt<int>(6);
    int minrot = opt<int>(7);
    int maxrot = opt<int>(8);
    int seed = opt<int>(9);
    int unique = opt<int>(10);
    rnd.setSeed(seed);
    int n = rnd.next(minn, maxn);
    int m = rnd.next(minm, maxm);
    string s;
    for (int i = 0; i < n; i++) {
        s.push_back(char(rnd.next('a', 'a'+unique-1)));
    }
    cout << s << endl;
    cout << m << endl;
    string t = rot(s, rnd.next(minrot, maxrot));
    for (int i = 0; i < m; i++) {
        int l = rnd.next(minl, maxl);
        l = min(l, n);
        int start = rnd.next(0, n-l);
        cout << t.substr(start, l) << endl;
    }
    return 0;
}