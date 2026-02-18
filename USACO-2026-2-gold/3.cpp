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

int N, F;
int a[500005], dep[500005];
vi rev[500005];
bool f[500005];
bool vis[500005];

int wai[500005];
bool cyc[500005];
int cnum[500005], cidx[500005];

vvt<bool> vs;
vvi vs2;

void dfs(int u) {
    int x = u;
    int y = u;
    while (1) {
        if (vis[x]) return;
        vis[x] = 1;
        x = a[x];
        y = a[a[y]];
        if (x == y) break;
    }

    int i = 0;
    while (1) {
        cnum[y] = sz(vs);
        cidx[y] = i++;
        cyc[y] = 1;
        y = a[y];
        if (x == y) break;
    }
    vt<bool> v(i*2);
    vi v2(i*2);
    vs.pb(v);
    vs2.pb(v2);
}

void dfs2(int u, int p, int d, int num, int idx) {
    cnum[u] = num;
    cidx[u] = idx;
    vt<bool> &v = vs[num];
    int n = sz(v)/2;
    if (f[u]) {
        int i = ((idx-d)%n+n)%n;
        v[i] = 1;
        v[i+n] = 1;
    }
    dep[u] = d;
    each(v,rev[u]) {
        if (v == p || cyc[v]) continue;
        dfs2(v, u, d+1, num, idx);
    }
}

void bfs() {
    rep(i,1,N+1) {
        wai[i] = INT_MAX;
    }
    queue<pii> q; // n, t
    rep(i,1,N+1) {
        if (f[i]) {
            q.push({i, 0});
        }
    }
    while (!q.empty()) {
        pii p = q.front(); q.pop();
        if (p.y >= wai[p.x]) continue;
        wai[p.x] = p.y;
        if (p.y+1 < wai[a[p.x]]) {
            q.push({a[p.x], p.y+1});
        }
    }
}

void solve(int c) {
    vt<bool> &v = vs[c];
    vi &v2 = vs2[c];
    int n = sz(v)/2;
    rep(i,0,n*2) v2[i] = 1e9;
    rrep(i,2*n-1,0) {
        if (!v[i]) {
            v2[i] = 0;
        } else if (i < 2*n-1 && v2[i+1] != 1e9) {
            v2[i] = v2[i+1]+1;
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> F;
    rep(i,1,N+1) {
        cin >> a[i];
        rev[a[i]].pb(i);
    }
    rep(i,0,F) {
        int x; cin >> x;
        f[x] = 1;
    }
    bfs();
    rep(i,1,N+1) {
        if (!vis[i]) {
            dfs(i);
        }
    }
    rep(i,1,N+1) {
        if (cyc[i]) {
            dfs2(i, -1, 0, cnum[i], cidx[i]);
        }
    }
    rep(i,0,sz(vs)) {
        solve(i);
    }
    // cout << "wai: " << endl;
    // rep(i,1,N+1) {
    //     cout << wai[i] << " ";
    // }
    // cout << endl;
    // cout << "cnum: " << endl;
    // rep(i,1,N+1) {
    //     cout << cnum[i] << " ";
    // }
    // cout << endl;
    // cout << "cidx: " << endl;
    // rep(i,1,N+1) {
    //     cout << cidx[i] << " ";
    // }
    // cout << endl;
    // cout << "vs2: " << endl;
    // rep(i,0,sz(vs2)) {
    //     cout << "cycle " << i << ": ";
    //     each(x,vs2[i]) {
    //         cout << x << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "dep: " << endl;
    // rep(i,1,N+1) {
    //     cout << dep[i] << " ";
    // }
    // cout << endl;
    vi ans(N+1);
    rep(i,1,N+1) {
        int num = cnum[i];
        int idx = cidx[i];
        vi &v2 = vs2[num];
        int n = sz(v2)/2;
        if (wai[i] == INT_MAX) {
            ans[i] = -2;
            continue;
        } else if (wai[i] == 0) {
            ans[i] = -1;
            continue;
        }
        int j = ((idx-(wai[i]-1)-dep[i])%n+n)%n;
        int res = wai[i]-1 - v2[j];
        // cout << "j: " << j << " res: " << res << endl;
        if (res < 0) {
            ans[i] = -1;
        } else {
            ans[i] = res;
        }
    }
    rep(i,1,N+1) {
        cout << ans[i] << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}