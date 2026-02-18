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
    if (a>=MOD2) a-=MOD2;
    return a;
}

inline int sub(int a, int b) {
    a-=b;
    if (a<0) a+=MOD2;
    return a;
}

inline int mul(int a, int b) {
    return (int)((a*1LL*b)%MOD2);
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
    return binpow(a, MOD2-2);
}

inline int di(int a, int b) {
    return mul(a, inv(b));
}

template <class T> class LzSegTree {
  private:
    const T DEFAULT = 0;
    vt<T> tr;
    vt<T> lz;
    int len;
    void push(int n) {
        if (lz[n] == 1) return;
        lz[2*n] = mul(lz[2*n], lz[n]);
        lz[2*n+1] = mul(lz[2*n+1], lz[n]);
        tr[2*n] = mul(tr[2*n], lz[n]);
        tr[2*n+1] = mul(tr[2*n+1], lz[n]);
        lz[n] = 1;
    }
    void _add(int n, int s, int e, int idx, T val) {
        if (s > idx || e < idx) return;
        if (s == e) {
            tr[n] = ad(tr[n], val);
            return;
        }
        int m = (s + e) / 2;
        push(n);
        _add(n*2, s, m, idx, val);
        _add(n*2+1, m+1, e, idx, val);
        tr[n] = ad(tr[n*2], tr[n*2+1]);
    }
    void _range_mul(int n, int s, int e, int l, int r, T val) {
        if (s > r || e < l) return;
        if (l <= s && e <= r) {
            tr[n] = mul(tr[n], val);
            lz[n] = mul(lz[n], val);
            return;
        }
        int m = (s + e) / 2;
        push(n);
        _range_mul(n*2, s, m, l, r, val);
        _range_mul(n*2+1, m+1, e, l, r, val);
        tr[n] = ad(tr[n*2], tr[n*2+1]);
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return DEFAULT;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        push(n);
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return ad(left, right);
    }
  public:
    LzSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, 1);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = ad(tr[i*2], tr[i*2+1]);
    }
    LzSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, 1);
    }
    void add(int idx, T val) {
        _add(1, 0, len-1, idx, val);
    }
    void range_mul(int l, int r, T val) {
        _range_mul(1, 0, len-1, l, r, val);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
};

int N, D;
pii a[1000005];
int l[1000005];
int ans[1000005];
vi b;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> D;
    rep(i,0,N) {
        cin >> a[i];
        if (a[i].y == 1) b.pb(a[i].x);
    }
    int j = 0;
    rep(i,0,N) {
        if (a[i].y == 0) {
            while (j < sz(b) && b[j] < a[i].x - D) j++;
            if (j == sz(b) || b[j] > a[i].x) l[i] = -1;
            else l[i] = j;
        }
    }
    LzSegTree<int> tr(sz(b));
    int sum = 1;
    j = 0;
    rep(i,0,N) {
        if (a[i].y == 1) {
            ans[i] = sum;
            tr.add(j, sum);
            sum = ad(sum, sum);
            j++;
        } else {
            if (l[i] == -1) {
                ans[i] = 0;
            } else {
                ans[i] = tr.query(l[i], j-1);
                tr.range_mul(l[i], j-1, 2);
                sum = ad(sum, ans[i]);
            }
        }
    }
    sum = sub(sum, 1);
    cout << sum << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}