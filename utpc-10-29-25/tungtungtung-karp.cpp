#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MOD 998244353
#define MOD2 1000000007
#define vt vector
template <class T> using vvt = vt<vt<T>>;
template <class T> using vvvt = vt<vvt<T>>;
template <class T> using vvvvt = vt<vvvt<T>>;
typedef vt<int> vi;
typedef vvt<int> vvi;
typedef vvvt<int> vvvi;
typedef vvvvt<int> vvvvi;
typedef vt<ll> vl;
typedef vvt<ll> vvl;
typedef vvvt<ll> vvvl;
typedef vvvvt<ll> vvvvl;
#define endl '\n'
#define pb push_back
#define pf push_front
#define all(x) x.begin(),x.end()
#define sz(x) (int)((x).size())
#define mset multiset
#define fi first
#define se second
#define rep(i,a,b) for(int i=a;i<b;i++)
#define repl(i,a,b) for(ll i=a;i<b;i++)
#define rrep(i,a,b) for(int i=a;i>=b;i--)
#define rrepl(i,a,b) for(ll i=a;i>=b;i--)
#define rep2(i,a,b,c) for(int i=a;i<b;i+=c)
#define repl2(i,a,b,c) for(ll i=a;i<b;i+=c)
#define rrep2(i,a,b,c) for(int i=a;i>=b;i-=c)
#define rrepl2(i,a,b,c) for(ll i=a;i>=b;i-=c)
#define each(i,a) for(auto &i:a)
#define yesno(x) cout<<(x?"YES":"NO")<<endl
struct pii {
    int x, y;
    bool operator<(const pii &a) const { return x == a.x ? y < a.y : x < a.x; }
    bool operator>(const pii &a) const { return x == a.x ? y > a.y : x > a.x; }
    bool operator==(const pii &a) const { return x == a.x && y == a.y; }
    bool operator!=(const pii &a) const { return x != a.x || y != a.y; }
    pii operator+(const pii &a) const { return {x+a.x, y+a.y}; }
    pii operator-(const pii &a) const { return {x-a.x, y-a.y}; }
    pii operator*(const int &a) const { return {x*a, y*a}; }
    pii operator/(const int &a) const { return {x/a, y/a}; }
    pii operator*(const pii &a) const { return {x*a.x, y*a.y}; }
    pii operator/(const pii &a) const { return {x/a.x, y/a.y}; }
    void operator+=(const pii &a) { x += a.x; y += a.y; }
    void operator-=(const pii &a) { x -= a.x; y -= a.y; }
    void operator*=(const int &a) { x *= a; y *= a; }
    void operator/=(const int &a) { x /= a; y /= a; }
    friend ostream& operator<<(ostream &os, const pii &p) {return os << "(" << p.x << ", " << p.y << ")";}
    friend istream& operator>>(istream &is, pii &p) {return is >> p.x >> p.y;}
};
struct pll {
    ll x, y;
    bool operator<(const pll &a) const { return x == a.x ? y < a.y : x < a.x; }
    bool operator>(const pll &a) const { return x == a.x ? y > a.y : x > a.x; }
    bool operator==(const pll &a) const { return x == a.x && y == a.y; }
    bool operator!=(const pll &a) const { return x != a.x || y != a.y; }
    pll operator+(const pll &a) const { return {x+a.x, y+a.y}; }
    pll operator-(const pll &a) const { return {x-a.x, y-a.y}; }
    pll operator*(const ll &a) const { return {x*a, y*a}; }
    pll operator/(const ll &a) const { return {x/a, y/a}; }
    pll operator*(const pll &a) const { return {x*a.x, y*a.y}; }
    pll operator/(const pll &a) const { return {x/a.x, y/a.y}; }
    void operator+=(const pll &a) { x += a.x; y += a.y; }
    void operator-=(const pll &a) { x -= a.x; y -= a.y; }
    void operator*=(const ll &a) { x *= a; y *= a; }
    void operator/=(const ll &a) { x /= a; y /= a; }
    friend ostream& operator<<(ostream &os, const pll &p) {return os << "(" << p.x << ", " << p.y << ")";}
    friend istream& operator>>(istream &is, pll &p) {return is >> p.x >> p.y;}
};
static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x^(x>>30))*0xbf58476d1ce4e5b9;
    x = (x^(x>>27))*0x94d049bb133111eb;
    return x^(x>>31);
}
struct custom_hash {
    static const uint64_t FIXED_RANDOM;
    size_t operator()(uint64_t x) const {return splitmix64(x + FIXED_RANDOM);}
    template<typename T> size_t operator()(const T& t) const {return splitmix64(uint64_t(std::hash<T>{}(t)) + FIXED_RANDOM);}
};
const uint64_t custom_hash::FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
mt19937 rng(custom_hash::FIXED_RANDOM);
template<typename K, typename V> using umap = unordered_map<K, V, custom_hash>;
template<typename K> using uset = unordered_set<K, custom_hash>;
template<typename T> using umset = unordered_multiset<T, custom_hash>;
template<class T> bool ckmin(T& a, const T& b) {
    return b < a ? a = b, 1 : 0; }
template<class T> bool ckmax(T& a, const T& b) {
    return a < b ? a = b, 1 : 0; }

struct yline {
    int y, x1, x2;
    bool operator<(const yline &a) const { return y == a.y ? x1 < a.x1 : y < a.y; }
};

int N, M;
vt<yline> ylines;
map<pii,int> m;
map<int,vt<pii>> xlines;
map<int,vi> ypoints;
int adj[250005][4];
bool vis[250005][4];

bool dfs(int u, int d = -1) {
    set<pii> s;
    queue<pii> q;
    q.push({u, d});
    // cout << "Starting DFS from node " << u << " in direction " << d << endl;
    while (!q.empty()) {
        pii curr = q.front(); q.pop();
        // cout << "Visiting node " << curr.x << " coming from direction " << curr.y << endl;
        int n = curr.x;
        int dir = curr.y;
        rep(d,0,4) {
            if (d != -1 && (d+2)%4 == dir) continue;
            int v = adj[n][d];
            if (v != -1) {
                if (vis[v][d]) {
                    if (s.count({v, d})) {
                        return 1;
                    }
                } else {
                    vis[v][d] = 1;
                    s.insert({v, d});
                    q.push({v, d});
                }
            }
        }
    }
    return 0;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N;
    rep(i,0,250005) {
        rep(j,0,4) {
            adj[i][j] = -1;
            vis[i][j] = 0;
        }
    }
    rep(i,0,N) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2) {
            xlines[x1].pb({y1, y2});
        } else {
            ylines.pb({y1, x1, x2});
        }
    }
    sort(all(ylines));
    each(x,xlines) {
        sort(all(x.se));
        each(p,x.se) {
            int prev = -1;
            each(yl,ylines) {
                if (yl.y >= p.x && yl.y <= p.y && yl.x1 <= x.fi && yl.x2 >= x.fi) {
                    m[{x.fi, yl.y}] = M;
                    // cout << "Mapping point (" << x.fi << ", " << yl.y << ") to node " << M << endl;
                    if (prev != -1) {
                        adj[prev][0] = M;
                        adj[M][2] = prev;
                        // cout << "Connecting node " << prev << " (down) to node " << M << " (up)" << endl;
                    }
                    prev = M;
                    ypoints[yl.y].pb(x.fi);
                    M++;
                }
            }
        }
    }
    each(y,ypoints) {
        sort(all(y.se));
        int i = 0;
        each(yl,ylines) {
            if (yl.y == y.fi) {
                int prev = -1;
                while (i < sz(y.se) && y.se[i] >= yl.x1 && y.se[i] <= yl.x2) {
                    int curr = m[{y.se[i], y.fi}];
                    if (prev != -1) {
                        adj[prev][1] = curr;
                        adj[curr][3] = prev;
                        // cout << "Connecting node " << prev << " (right) to node " << curr << " (left)" << endl;
                    }
                    prev = curr;
                    i++;
                }
            }
        }
    }
    bool ans = 0;
    rep(i,0,M) {
        rep(d,0,4) {
            if (!vis[i][d]) {
                ans |= dfs(i);
            }
        }
    }
    yesno(ans);

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}