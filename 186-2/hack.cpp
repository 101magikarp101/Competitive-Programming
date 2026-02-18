#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    int n = 5000;
    cout << t << "\n";
    cout << n << "\n";

    for (int i = 0; i < n; i++) cout << 1 << (i+1==n?'\n':' ');
    for (int i = 0; i < n; i++) cout << 2 << (i+1==n?'\n':' ');
    for (int i = 0; i < n; i++) cout << 3 << (i+1==n?'\n':' ');

    return 0;
}
