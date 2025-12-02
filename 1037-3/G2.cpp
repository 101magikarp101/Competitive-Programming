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

template <class T> class LzSegTree {
  private:
    const T DEFAULT = INT_MIN;
    vt<T> tr;
    vt<T> lz;
    int len;
    void push(int n) {
        if (lz[n] == 0) return;
        lz[2*n] += lz[n];
        lz[2*n+1] += lz[n];
        tr[2*n] += lz[n];
        tr[2*n+1] += lz[n];
        lz[n] = 0;
    }
    void _range_add(int n, int s, int e, int l, int r, T val) {
        if (s > r || e < l) return;
        if (l <= s && e <= r) {
            tr[n] += val;
            lz[n] += val;
            return;
        }
        int m = (s + e) / 2;
        push(n);
        _range_add(n*2, s, m, l, r, val);
        _range_add(n*2+1, m+1, e, l, r, val);
        tr[n] = max(tr[n*2], tr[n*2+1]);
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return DEFAULT;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        push(n);
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return max(left, right);
    }
  public:
    LzSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, 0);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = max(tr[i*2], tr[i*2+1]);
    }
    LzSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, 0);
    }
    void range_add(int l, int r, T val) {
        _range_add(1, 0, len-1, l, r, val);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
};

int T, N;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N;
        vi a(N), b(N);
        vt<pii> c(N);
        rep(i,0,N) {
            cin >> a[i];
            c[i] = {a[i], i};
        }
        sort(all(c));
        int ans = 0;
        int x = 0;
        vi p(N), s(N);
        rep(i,0,N) {
            p[i] = i+1;
            s[i] = N-i;
        }
        LzSegTree<int> stp(p), sts(s);
        rep(i,0,N) {
            int v = c[i].x;
            int idx = c[i].y;
            while (x < N) {
                int y = c[x].y;
                int r = stp.query(y, N-1)-(y==0?0:stp.query(y-1,y-1));
                int l = sts.query(0, y)-sts.query(y,y);
                if (l+r < 0) {
                    x++;
                } else {
                    break;
                }
            }
            ckmax(ans, v-c[x].x);
            stp.range_add(idx, N-1, -2);
            sts.range_add(0, idx, -2);
        }
        cout << ans << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}