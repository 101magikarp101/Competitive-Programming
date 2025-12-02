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

int N, K;
ll a[200005];
vi adj[200005];
vvl dp[200005];

vl merge(vl &a, vl &b) {
    vl res(K+1,0);
    rep(i,0,K+1) {
        rep(j,0,K+1-i) {
            ckmax(res[i+j], a[i] + b[j]);
        }
    }
    return res;
}

void dfs(int u, int p) {
    // cerr << "dfs(" << u << ", " << p << ")" << endl;
    dp[u] = vvl(2, vl(K+1, 0));
    vl tot(K+1, 0);
    vvvl vs(2);
    vvl pre(2), suf(2);
    each(v, adj[u]) {
        if (v == p) continue;
        dfs(v, u);
        tot = merge(tot, dp[v][0]);
        vs[0].pb(dp[v][0]);
        vs[1].pb(dp[v][1]);
    }
    rep(i,0,K+1) {
        ckmax(dp[u][0][i], tot[i]);
    }
    int n = sz(vs[0]);
    if (n == 0) {
        dp[u][0][1] = a[u];
        dp[u][1][0] = a[u];
        return;
    }
    pre = vvl(n, vl(K+1, 0));
    suf = vvl(n, vl(K+1, 0));
    pre[0] = vs[0][0];
    rep(i,1,n) {
        pre[i] = merge(pre[i-1], vs[0][i]);
    }
    suf[n-1] = vs[0][n-1];
    rrep(i,n-2,0) {
        suf[i] = merge(suf[i+1], vs[0][i]);
    }
    rep(i,0,n) {
        vl cur = vs[1][i];
        if (i) cur = merge(pre[i-1], cur);
        if (i < n-1) cur = merge(cur, suf[i+1]);
        rep(j,0,K+1) {
            ckmax(dp[u][1][j], cur[j]+a[u]);
        }
        rep(j,1,K+1) {
            ckmax(dp[u][0][j], cur[j-1]+a[u]);
        }
    }

    if (n < 2) return;
    vvl pre2(n, vl(K+1, 0));
    vvl suf2(n, vl(K+1, 0));
    pre2[0] = vs[1][0];
    rep(i,1,n) {
        pre2[i] = merge(pre2[i-1], vs[0][i]);
        vl hm = merge(pre[i-1], vs[1][i]);
        rep(j,0,K+1) {
            ckmax(pre2[i][j], hm[j]);
        }
    }
    suf2[n-1] = vs[1][n-1];
    rrep(i,n-2,0) {
        suf2[i] = merge(suf2[i+1], vs[0][i]);
        vl hm = merge(suf[i+1], vs[1][i]);
        rep(j,0,K+1) {
            ckmax(suf2[i][j], hm[j]);
        }
    }

    rep(i,0,n-1) {
        vl cur = merge(pre2[i], suf2[i+1]);
        rep(j,1,K+1) {
            ckmax(dp[u][0][j], cur[j-1]+a[u]);
        }
    }
    set<int> st;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> K;
    rep(i,1,N+1) cin >> a[i];
    rep(i,0,N-1) {
        int u, v; cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    dfs(1,0);
    // cout << "dp" << endl;
    // rep(i,1,N+1) {
    //     cout << "dp[" << i << "][0]: ";
    //     rep(j,0,K+1) {
    //         cout << dp[i][0][j] << " ";
    //     }
    //     cout << endl;
    //     cout << "dp[" << i << "][1]: ";
    //     rep(j,0,K+1) {
    //         cout << dp[i][1][j] << " ";
    //     }
    //     cout << endl;
    // }
    ll ans = 0;
    rep(i,0,K+1) {
        ckmax(ans, dp[1][0][i]);
    }
    cout << ans << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}