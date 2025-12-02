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

struct state {
    ll t, p, s, x;
    state operator+(const state &a) const {
        return {max((ll)-1e18, t+a.t), max(p, t+a.p), max(a.s, s+a.t), max({x, a.x, s+a.p})};
    }
};

const state DEFAULT = {0,0,0,0};
const state INF = {(ll)-1e18,(ll)-1e18,(ll)-1e18,(ll)-1e18};

template <class T = state> class LzSegTree {
  private:
    // const T DEFAULT = {(ll)-1e18,(ll)-1e18,(ll)-1e18,(ll)-1e18};
    vt<T> tr;
    int len;
    void _set(int n, int s, int e, int idx, T val) {
        if (s > idx || e < idx) return;
        if (s == e) {
            tr[n] = val;
            return;
        }
        int m = (s + e) / 2;
        _set(n*2, s, m, idx, val);
        _set(n*2+1, m+1, e, idx, val);
        tr[n] = tr[n*2] + tr[n*2+1];
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return INF;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return left + right;
    }
  public:
    LzSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = tr[i*2] + tr[i*2+1];
    }
    LzSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
    }
    void set(int idx, T val) {
        _set(1, 0, len-1, idx, val);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
};

struct artist {
    int i; ll v;
};

struct query {
    int l, r, i;
};

int T, N, M, Q;
vt<artist> beg[200005], en[200005];
vt<query> qs[200005];
ll ans[200005];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N >> M >> Q;
        rep(i,1,N+1) {
            beg[i].clear();
            en[i].clear();
            qs[i].clear();
        }
        LzSegTree<state> st(M);
        rep(i,0,M) {
            int l, r; ll v; cin >> l >> r >> v;
            beg[l].pb({i,v});
            en[r].pb({i,v});
        }
        rep(i,0,Q) {
            int l, r, k; cin >> l >> r >> k;
            l--, r--;
            qs[k].pb({l,r,i});
        }
        rep(i,1,N+1) {
            each(a,beg[i]) {
                st.set(a.i, {a.v, a.v, a.v, a.v});
                // cout << "add " << a.i << " " << a.v << endl;
                // cout << "st2: " << st2.query(a.i, a.i+1) << endl;
            }
            each(q,qs[i]) {
                ll res = st.query(q.l, q.r).x;
                // cout << "res2: " << res2 << endl;
                ans[q.i] = res;
                // cout << "query " << q.l << " " << q.r << " = " << ans[q.i] << endl;
            }
            each(a,en[i]) {
                st.set(a.i, DEFAULT);
                // cout << "rem " << a.i << " " << a.v << endl;
            }
        }
        rep(i,0,Q) cout << ans[i] << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}