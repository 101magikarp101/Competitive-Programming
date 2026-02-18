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

struct Line {
    ll a = 0, b = 4e18; // y = a*x + b
    ll get(ll x) {
        return a*x + b;
    }
};

struct LiChao {
    private:
    vt<Line> tr;
    vl xs;
    int len;
    bool comp = false;
    // insert new line l in node n covering [s, e)
    void _insert(int n, int s, int e, Line l) {
        int m = (s+e)/2;
        bool left = l.get(s) < tr[n].get(s);
        bool mid = l.get(m) < tr[n].get(m);
        if (mid) {
            swap(tr[n], l);
        }
        if (e - s == 1) {
            return;
        } else if (left != mid) {
            _insert(n*2, s, m, l);
        } else {
            _insert(n*2+1, m, e, l);
        }
    }
    // insert new line segment l of [sl, sr) in node covering [s, e)
    void _insert_seg(int n, int s, int e, Line l, int sl, int sr) {
        if (sr <= s || e <= sl) return;
        if (sl <= s && e <= sr) {
            _insert(n, s, e, l);
            return;
        }
        if (e - s == 1) {
            _insert(n, s, e, l);
            return;
        }
        int m = (s+e)/2;
        _insert_seg(n*2, s, m, l, sl, sr);
        _insert_seg(n*2+1, m, e, l, sl, sr);
    }
    ll _get(int n, int s, int e, int x) {
        int m = (s+e)/2;
        ll ret = tr[n].get(comp ? xs[x] : x);
        if (e - s == 1) {
            return ret;
        } else if (comp ? xs[x] < xs[m] : x < m) {
            ckmin(ret, _get(n*2, s, m, x));
        } else {
            ckmin(ret, _get(n*2+1, m, e, x));
        }
        return ret;
    }
    public:
    LiChao(vl &vals) {
        xs = vals;
        comp = true;
        len = 1;
        while (len < sz(xs)) len *= 2;
        tr.assign(len*2+5, Line());
    }
    // make lichao for n points [0, n)
    LiChao(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2+5, Line());
    }
    void insert(Line l) {
        _insert(1, 0, len, l);
    }
    void insert_seg(Line l, int sl, int sr) {
        _insert_seg(1, 0, len, l, sl, sr);
    }
    ll get(int x) {
        return _get(1, 0, len, x);
    }
};

int N, M;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> M;
    LiChao lichao(M+1);
    rep(i,0,N) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        ll a = (y2 - y1) / (x2 - x1);
        ll b = y1 - a * x1;
        Line line = {-a, -b};
        lichao.insert_seg(line, x1, x2+1);
    }
    rep(x,0,M+1) {
        ll res = -lichao.get(x);
        if (res <= -4e18) {
            cout << -1 << ' ';
        } else {
            cout << res << ' ';
        }
    }
    cout << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}