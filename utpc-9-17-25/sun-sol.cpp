#include <bits/stdc++.h>
using namespace std;

int N, K;
int a[200005];
int pre1[200005], pre2[200005];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> K;
    for (int i = 1; i <= N; i++) {
        cin >> a[i];
        pre1[i] = pre1[i - 1] + (i%2==1 ? 1 : 2) * a[i];
        pre2[i] = pre2[i - 1] + (i%2==1 ? 2 : 1) * a[i];
    }
    for (int i = 0; i < K; i++) {
        int l, r;
        cin >> l >> r;
        int ans = -1;
        if (l % 2 == 1) ans = pre1[r] - pre1[l - 1];
        else ans = pre2[r] - pre2[l - 1];
        cout << ans << '\n';
    }

    return 0;
}