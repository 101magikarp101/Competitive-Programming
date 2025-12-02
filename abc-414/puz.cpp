#include <bits/stdc++.h>
using namespace std;

string solve(string s) {
    int N = s.size();
    string fin;
    // for (int i = 0; i < N; i++) {
    //     cout << s[i] << "\t";
    //     if (i%8==7) cout << endl;
    // }
    for (int i = 0; i < N; i+=8) {
        // fin += s.substr(i,1);
        string t = s.substr(i, 8);
        int x = stoi(t, nullptr, 2);
        x ^= (1<<3);

        x ^= (1<<0);
        x ^= (1<<2);
        x ^= (1<<4);
        x ^= (1<<6);
    
        x ^= ((1<<8)-1);
        cout << bitset<8>(x) << "\t";

        string res = bitset<8>(x).to_string();
        reverse(res.begin(), res.end());
        x = stoi(res, nullptr, 2);
        cout << bitset<8>(x) << "\t";

        int sus = ((x>>4)&1);
        x = (x&((1<<8)-1-(1<<4))) | (((x>>0)&1)<<4);
        x = (x&((1<<8)-1-(1<<0))) | (((x>>1)&1)<<0);
        x = (x&((1<<8)-1-(1<<1))) | (sus<<1);
        cout << bitset<8>(x) << "\t";
        // cout << (char)(x&((1<<7)-1)) << endl;
        fin += (char)('0'+((x>>7)&1));
        x &= ((1<<7)-1);
        for (int j = 0; j < 7; j++) {
            cout << ((x>>j)&1) << "\t";
        }
        cout << (char)x << endl;
    }
    return fin;
}

void solve2(string s) {
    int N = s.size();

}

int main() {
    string s; cin >> s;
    string fin;
    int N = s.size();
    // fin = solve(s);
    for (int i = 0; i < N; i+=8) {
        cout << s[i] << "\t";
        if (i%64==56) cout << endl;
    }
    // cout << fin << endl;
    // solve(fin);
    // 1 - 11 reg "Flip bit #5"
    // 12 - 25 bit 5 "Also even bits"
    // 26 - 33 5 + evens "Inverse-"
    // 35 - 42 (5+evens) inverse "Reverse-"
    // 40 - 49 "4->7->8->4"
}