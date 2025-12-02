#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n; cin >> n;
    for (int i = 0; i < 8; i++) {
        if ((10*n + i) % 8 == 0) {
            cout << i << endl;
            break;
        }
    }

    return 0;
}