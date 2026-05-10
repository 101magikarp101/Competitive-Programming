#include <iostream>
#include <algorithm>
#include <queue>
#include <climits>
 
using namespace std;
 
const int MAXN = 200005;
 
int n, m, k;
pair<int, int> weights[MAXN]; // weight-frequency pairs
int main() {
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++) {
        cin >> weights[i].first >> weights[i].second; // weight, then frequency
    }
    // sort in decreasing order of weight
    sort(weights, weights + n);
    reverse(weights, weights + n);
    // create a queue to store the current tower status
    queue<pair<int, int>> towers;
    towers.push(make_pair(INT_MAX, m));
    long long ans = 0;
    // push things into queue from largest to smallest
    for (int i = 0; i < n; i++) {
        int rem = weights[i].second; // number of cows remaining
        while (!towers.empty() && (rem > 0 && towers.front().first >= weights[i].first + k)) {
            if (towers.front().second >= rem) {
                towers.front().second -= rem;
                rem = 0;
            } else {
                rem -= towers.front().second;
                towers.pop();
            }
        }
        int cur = weights[i].second - rem;
        ans += cur;
        if (cur > 0) {
            towers.push(make_pair(weights[i].first, cur));
        }
    }
    cout << ans << endl;
}