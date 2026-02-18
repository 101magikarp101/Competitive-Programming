#include <bits/stdc++.h>
using namespace std;

struct State {
    int v;
    int src;
    int d;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> shops(k);
    vector<char> isShop(n + 1, false);
    for (int i = 0; i < k; i++) {
        cin >> shops[i];
        isShop[shops[i]] = true;
    }

    vector<vector<int>> adj(n + 1);
    adj.reserve(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    const int INF = 1e9;
    vector<int> dist1(n + 1, INF), dist2(n + 1, INF);
    vector<int> src1(n + 1, -1), src2(n + 1, -1);

    deque<State> q;

    // Initialize BFS from every shop as a source
    for (int s : shops) {
        if (src1[s] != s) {           // avoid duplicate pushes if input repeats a shop
            src1[s] = s;
            dist1[s] = 0;
            q.push_back({s, s, 0});
        }
    }

    auto relax = [&](int u, int nd, int src) {
        if (src1[u] == src) {
            if (nd < dist1[u]) {
                dist1[u] = nd;
                q.push_back({u, src, nd});
            }
            return;
        }
        if (src2[u] == src) {
            if (nd < dist2[u]) {
                dist2[u] = nd;
                q.push_back({u, src, nd});
            }
            return;
        }
        if (nd < dist1[u]) {
            dist2[u] = dist1[u];
            src2[u]  = src1[u];
            dist1[u] = nd;
            src1[u]  = src;
            q.push_back({u, src, nd});
        } else if (nd < dist2[u]) {
            dist2[u] = nd;
            src2[u]  = src;
            q.push_back({u, src, nd});
        }
    };

    while (!q.empty()) {
        State cur = q.front();
        q.pop_front();

        int v = cur.v, src = cur.src, d = cur.d;

        // Skip stale queue entries
        if (src1[v] == src) {
            if (d != dist1[v]) continue;
        } else if (src2[v] == src) {
            if (d != dist2[v]) continue;
        } else {
            continue;
        }

        for (int u : adj[v]) {
            relax(u, d + 1, src);
        }
    }

    for (int v = 1; v <= n; v++) {
        int ans;
        if (isShop[v]) ans = (dist2[v] >= INF ? -1 : dist2[v]); // nearest *other* shop
        else           ans = (dist1[v] >= INF ? -1 : dist1[v]); // nearest shop
        cout << ans << (v == n ? '\n' : ' ');
    }
    return 0;
}
