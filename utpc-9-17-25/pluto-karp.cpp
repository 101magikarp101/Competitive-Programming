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

template <class T> class CntSegTree {
  private:
    const T DEFAULT = 0;
    vt<T> tr;
    vt<bool> lz;
    int len;
    void push(int n, int s, int m, int e) {
        if (!lz[n]) return;
        lz[2*n] = !lz[2*n];
        lz[2*n+1] = !lz[2*n+1];
        tr[2*n] = (m-s+1) - tr[2*n];
        tr[2*n+1] = (e-m) - tr[2*n+1];
        lz[n] = false;
    }
    void _range_flip(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return;
        if (l <= s && e <= r) {
            tr[n] = (e-s+1) - tr[n];
            lz[n] = !lz[n];
            return;
        }
        int m = (s + e) / 2;
        push(n, s, m, e);
        _range_flip(n*2, s, m, l, r);
        _range_flip(n*2+1, m+1, e, l, r);
        tr[n] = tr[n*2] + tr[n*2+1];
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return DEFAULT;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        push(n, s, m, e);
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return left + right;
    }
  public:
    CntSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, false);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = tr[i*2] + tr[i*2+1];
    }
    CntSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, false);
    }
    void range_flip(int l, int r) {
        _range_flip(1, 0, len-1, l, r);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
};

template <class T> class LzSegTree {
  private:
    const T DEFAULT = {0,0};
    vt<pii> tr; // zeroes, ones
    vt<pii> lz; // swap, negate
    int len;
    void push(int n, int s, int m, int e) {
        if (!lz[n].x && !lz[n].y) return;
        if (lz[n].x) {
            swap(tr[2*n].x, tr[2*n].y);
            swap(tr[2*n+1].x, tr[2*n+1].y);
            lz[2*n].x = !lz[2*n].x;
            lz[2*n+1].x = !lz[2*n+1].x;
        }
        if (lz[n].y) {
            lz[2*n].y = !lz[2*n].y;
            lz[2*n+1].y = !lz[2*n+1].y;
            tr[2*n] = tr[2*n]*(-1);
            tr[2*n+1] = tr[2*n+1]*(-1);
        }
        lz[n] = {false, false};
    }
    void _range_flip(int n, int s, int e, int l, int r, bool flip, bool neg) {
        if (s > r || e < l) return;
        if (l <= s && e <= r) {
            if (flip) {
                swap(tr[n].x, tr[n].y);
                lz[n].x = !lz[n].x;
            }
            if (neg) {
                tr[n] = tr[n]*(-1);
                lz[n].y = !lz[n].y;
            }
            return;
        }
        int m = (s + e) / 2;
        push(n, s, m, e);
        _range_flip(n*2, s, m, l, r, flip, neg);
        _range_flip(n*2+1, m+1, e, l, r, flip, neg);
        tr[n] = tr[n*2] + tr[n*2+1];
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return DEFAULT;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        push(n, s, m, e);
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return left + right;
    }
  public:
    LzSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, pii{0,0});
        lz.assign(len*2, pii{0,0});
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = tr[i*2] + tr[i*2+1];
    }
    LzSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, pii{0,0});
        lz.assign(len*2, pii{0,0});
    }
    void range_flip(int l, int r, bool flip, bool neg) {
        _range_flip(1, 0, len-1, l, r, flip, neg);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
};

int N, Q;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> Q;
    string s; cin >> s;
    vi a(N);
    rep(i,0,N) a[i] = (s[i]=='1'?1:0);
    CntSegTree<int> seg(a);
    vt<pii> b(N);
    bool o = 0, z = 0;
    rep(i,0,N) {
        if (a[i] == 1) {
            if (o) b[i] = {0, i};
            else b[i] = {0, -i};
            o = !o;
        } else {
            if (z) b[i] = {i, 0};
            else b[i] = {-i, 0};
            z = !z;
        }
    }
    LzSegTree<pii> seg2(b);
    rep(i,0,Q) {
        int l, r; cin >> l >> r; l--; r--;
        bool neg = l%2;
        bool neg2 = (r+1)%2;
        seg.range_flip(l, r);
        seg2.range_flip(l, r, true, false);
        if (neg) seg2.range_flip(l, N-1, false, true);
        if (r+1 < N && neg2) seg2.range_flip(r+1, N-1, false, true);
        int cnt = seg.query(0, N-1);
        pii ans = seg2.query(0, N-1);
        if (cnt%2==0) {
            cout << ans.y << endl;
        } else {
            cout << ans.x+1 << endl;
        }
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}