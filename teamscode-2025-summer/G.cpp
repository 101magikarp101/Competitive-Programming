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

template <class T> class MaxSegTree {
private:
    const T DEFAULT = 1e18;
    vt<T> tr;
    int len;
public:
    MaxSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
    }
    MaxSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = min(tr[i*2], tr[i*2+1]);
    }
    void add(int i, T v) {
        i += len;
        ckmin(tr[i], v);
        for (i >>= 1; i > 0; i >>= 1) {
            tr[i] = min(tr[i*2], tr[i*2+1]);
        }
    }
    // query range [l, r)
    T query(int l, int r) {
        l += len;
        r += len;
        T res = DEFAULT;
        while (l < r) {
            if (l & 1) ckmin(res, tr[l++]);
            if (r & 1) ckmin(res, tr[--r]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

struct edge {
    int v; ll w;
};

struct state {
    int u; ll d;
    bool operator<(const state &a) const {
        return d > a.d;
    }
};

struct point {
    int i, y; ll d;
};

int N, M;
pll a[200005];
vt<point> b[200005];
vt<edge> adj[200005];
ll d[200005];
map<int,ll> mx, my;
map<ll,int> rx, ry;
int n, m;

void comp() {
    set<ll> sx, sy;
    rep(i,1,2*N+1) {
        sx.insert(a[i].x);
        sy.insert(a[i].y);
    }
    each(x,sx) {
        mx[sz(mx)] = x;
        rx[x] = sz(rx);
    }
    n = sz(mx);
    each(y,sy) {
        my[sz(my)] = y;
        ry[y] = sz(ry);
    }
    m = sz(my);
    rep(i,1,2*N+1) {
        b[rx[a[i].x]].pb({(i-1)/N, ry[a[i].y], d[i]});
    }
}

void dijk(int s) {
    priority_queue<state> pq;
    d[s] = 0;
    pq.push({s, 0});
    while (!pq.empty()) {
        auto [u, dist] = pq.top(); pq.pop();
        if (dist > d[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push({v, d[v]});
            }
        }
    }
}

ll solve() {
    MaxSegTree<ll> stu(m), std(m);
    ll ans = LLONG_MAX;
    rrep(i,n-1,0) {
        each(x,b[i]) {
            if (x.i == 1) {
                stu.add(x.y, x.d+mx[i]+my[x.y]);
                std.add(x.y, x.d+mx[i]-my[x.y]);
            }
        }
        each(x,b[i]) {
            if (x.i == 0) {
                ll cur = x.d-mx[i]-my[x.y];
                ll res = stu.query(x.y,m)+cur;
                ckmin(ans, res);
                cur = x.d-mx[i]+my[x.y];
                res = std.query(0,x.y+1)+cur;
                ckmin(ans, res);
            }
        }
    }
    MaxSegTree<ll> stu2(m), std2(m);
    rep(i,0,n) {
        each(x,b[i]) {
            if (x.i == 1) {
                stu2.add(x.y, x.d-mx[i]+my[x.y]);
                std2.add(x.y, x.d-mx[i]-my[x.y]);
            }
        }
        each(x,b[i]) {
            if (x.i == 0) {
                ll cur = x.d+mx[i]-my[x.y];
                ll res = stu2.query(x.y,m)+cur;
                ckmin(ans, res);
                cur = x.d+mx[i]+my[x.y];
                res = std2.query(0,x.y+1)+cur;
                ckmin(ans, res);
            }
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> M;
    rep(i,1,2*N+1) d[i] = LLONG_MAX;
    rep(i,1,2*N+1) cin >> a[i];
    rep(i,0,M) {
        int u, v; ll w; cin >> u >> v >> w;
        adj[u].pb({v, w});
        adj[v].pb({u, w});
    }
    dijk(1);
    dijk(2*N);
    comp();
    ll ans = solve();
    cout << ans << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}