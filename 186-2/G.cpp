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

inline int ad(int a, int b) {
    a+=b;
    if (a>=MOD) a-=MOD;
    return a;
}

inline int sub(int a, int b) {
    a-=b;
    if (a<0) a+=MOD;
    return a;
}

inline int mul(int a, int b) {
    return (int)((a*1LL*b)%MOD);
}

inline int binpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b&1) res = mul(res, a);
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

inline int inv(int a) {
    return binpow(a, MOD-2);
}

inline int di(int a, int b) {
    return mul(a, inv(b));
}

struct Node;
int nnode();

vt<Node*> nodes;
int ncnt = 0;

int updlz(int &lz, int v) {
    if (lz == -1) lz = 1;
    lz = mul(lz, v);
    return lz;
}

struct Node {
    int v = 0;
    int lz = -1;
    int l = -1, r = -1;
    void push() {
        if (lz == -1) return;
        if (l != -1) {
            nodes[l]->v = mul(nodes[l]->v, lz);
            updlz(nodes[l]->lz, lz);
        }
        if (r != -1) {
            nodes[r]->v = mul(nodes[r]->v, lz);
            updlz(nodes[r]->lz, lz);
        }
        lz = -1;
    }
    int get(int i, int lo, int hi) {
        if (lo == hi) return v;
        push();
        int mid = (lo+hi)/2;
        if (i <= mid) {
            if (l == -1) return 0;
            return nodes[l]->get(i, lo, mid);
        } else {
            if (r == -1) return 0;
            return nodes[r]->get(i, mid+1, hi);
        }
    }
    void range_mul(int ql, int qr, int val, int lo, int hi) {
        if (qr < lo || hi < ql) return;
        if (ql <= lo && hi <= qr) {
            v = mul(v, val);
            updlz(lz, val);
            return;
        }
        push();
        int mid = (lo+hi)/2;
        if (l == -1) l = nnode();
        nodes[l]->range_mul(ql, qr, val, lo, mid);
        if (r == -1) r = nnode();
        nodes[r]->range_mul(ql, qr, val, mid+1, hi);
        v = ad(nodes[l]->v, nodes[r]->v);
    }
    void add(int val, int i, int lo, int hi) {
        if (lo == hi) {
            v = ad(v, val);
            return;
        }
        push();
        int mid = (lo+hi)/2;
        if (i <= mid) {
            if (l == -1) l = nnode();
            nodes[l]->add(val, i, lo, mid);
        } else {
            if (r == -1) r = nnode();
            nodes[r]->add(val, i, mid+1, hi);
        }
        v = ad(l == -1 ? 0 : nodes[l]->v, r == -1 ? 0 : nodes[r]->v);
    }
    int sum(int ql, int qr, int lo, int hi) {
        if (qr < lo || hi < ql) return 0;
        if (ql <= lo && hi <= qr) return v;
        push();
        int mid = (lo+hi)/2;
        int res = 0;
        if (l != -1) res = ad(res, nodes[l]->sum(ql, qr, lo, mid));
        if (r != -1) res = ad(res, nodes[r]->sum(ql, qr, mid+1, hi));
        return res;
    }
    void merge(Node* o, int lo, int hi) {
        if (!o) return;
        if (lo == hi) {
            v = ad(v, o->v);
            return;
        }
        push();
        o->push();
        int mid = (lo+hi)/2;
        if (o->l != -1) {
            if (l == -1) l = nnode();
            nodes[l]->merge(nodes[o->l], lo, mid);
        }
        if (o->r != -1) {
            if (r == -1) r = nnode();
            nodes[r]->merge(nodes[o->r], mid+1, hi);
        }
        v = ad(l == -1 ? 0 : nodes[l]->v, r == -1 ? 0 : nodes[r]->v);
    }
    void print(int lo, int hi) {
        cout << "[" << lo << ", " << hi << "]: " << v << " (lz: " << lz << ")" << endl;
        int m = (lo+hi)/2;
        if (l != -1) nodes[l]->print(lo, m);
        if (r != -1) nodes[r]->print(m+1, hi);
    }
};

int nnode() {
    nodes.pb(new Node());
    return ncnt++;
}

int T, N, K;
vi adj[300005];
int d[300005], siz[300005];
int maxd[300005];
int ns[300005];
int fac[300005];

void pre(int n) {
    fac[0] = 1;
    rep(i,1,n+1) {
        fac[i] = mul(fac[i-1], i);
    }
}

void dfs(int u, int dep) {
    siz[u] = 1;
    d[u] = dep;
    maxd[u] = dep;
    each(v, adj[u]) {
        dfs(v, dep+1);
        siz[u] += siz[v];
        ckmax(maxd[u], maxd[v]);
    }
    int n = sz(adj[u]);
    rep(i,0,n) {
        if (siz[adj[u][i]] > siz[adj[u][0]]) {
            swap(adj[u][i], adj[u][0]);
        }
    }
    if (n == 0) {
        ns[u] = nnode();
        nodes[ns[u]]->add(1, d[u], 0, N);
        // cout << "ns[" << u << "]: " << ns[u] << endl;
        // nodes[ns[u]]->print(0, N);
        return;
    }
    ns[u] = ns[adj[u][0]];
    // cout << "ns[" << u << "]: " << ns[u] << endl;
    vi hm(n);
    int l = d[u];
    int r = d[u]+K-1;
    ckmin(r, N);
    rep(i,0,n) {
        hm[i] = nodes[ns[adj[u][i]]]->sum(l, r, 0, N);
        // cout << "hm[" << adj[u][i] << "]: " << hm[i] << endl;
    }
    vi pre(n), suf(n);
    pre[0] = hm[0];
    rep(i,1,n) {
        pre[i] = mul(pre[i-1], hm[i]);
    }
    suf[n-1] = hm[n-1];
    rrep(i,n-2,0) {
        suf[i] = mul(suf[i+1], hm[i]);
    }

    rep(i,0,n) {
        int val = 1;
        if (i > 0) val = mul(val, pre[i-1]);
        if (i+1 < n) val = mul(val, suf[i+1]);
        val = mul(val, fac[n-1]);
        nodes[ns[adj[u][i]]]->range_mul(0, N, val, 0, N);
    }
    rep(i,1,n) {
        nodes[ns[u]]->merge(nodes[ns[adj[u][i]]], 0, N);
    }
    // cout << "ns[" << u << "]: " << ns[u] << endl;
    // nodes[ns[u]]->print(0, N);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    pre(300000);

    cin >> T;
    while (T--) {
        cin >> N >> K;
        // cout << "N: " << N << ", K: " << K << endl;
        nodes.clear();
        ncnt = 0;
        rep(i,1,N+1) {
            adj[i].clear();
        }
        rep(i,2,N+1) {
            int p; cin >> p;
            adj[p].pb(i);
        }
        dfs(1, 0);
        // cout << "ns[1]: " << ns[1] << endl;
        int ans = nodes[ns[1]]->sum(0, N, 0, N);
        cout << ans << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}