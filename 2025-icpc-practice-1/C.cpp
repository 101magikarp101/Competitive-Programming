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

struct frac {
    ll n, d;
    frac(ll nn=0, ll dd=1) {
        if (dd < 0) {
            nn = -nn;
            dd = -dd;
        }
        if (nn == 0) {
            n = 0;
            d = 1;
            return;
        }
        n = nn;
        d = dd;
        // ll g = gcd(abs(nn), dd);
        // n = nn/g;
        // d = dd/g;
    }
    frac(pll p) : frac(p.x, p.y) {}
    frac operator+(const frac &a) const {
        ll nn = n*a.d + d*a.n;
        if (nn == 0) return {0,1};
        ll dd = d*a.d;
        if (dd < 0) {
            nn = -nn;
            dd = -dd;
        }
        return {nn, dd};
        // ll g = gcd(abs(nn), dd);
        // return {nn/g, dd/g};
    }
    frac operator-(const frac &a) const {
        ll nn = n*a.d - d*a.n;
        if (nn == 0) return {0,1};
        ll dd = d*a.d;
        if (dd < 0) {
            nn = -nn;
            dd = -dd;
        }
        return {nn, dd};
        // ll g = gcd(abs(nn), dd);
        // return {nn/g, dd/g};
    }
    frac operator*(const frac &a) const {
        ll nn = n*a.n;
        if (nn == 0) return {0,1};
        ll dd = d*a.d;
        if (dd < 0) {
            nn = -nn;
            dd = -dd;
        }
        return {nn, dd};
        // ll g = gcd(abs(nn), dd);
        // return {nn/g, dd/g};
    }
    frac operator/(const frac &a) const {
        ll nn = n*a.d;
        if (nn == 0) return {0,1};
        ll dd = d*a.n;
        if (dd < 0) {
            nn = -nn;
            dd = -dd;
        }
        return {nn, dd};
        // ll g = gcd(abs(nn), dd);
        // return {nn/g, dd/g};
    }
    void red() {
        ll g = gcd(abs(n), d);
        n /= g;
        d /= g;
    }
    bool operator<(const frac &a) const {
        return n*a.d < d*a.n;
    }
    bool operator>(const frac &a) const {
        return n*a.d > d*a.n;
    }
    bool operator==(const frac &a) const {
        return n*a.d == d*a.n;
    }
    bool operator!=(const frac &a) const {
        return n*a.d != d*a.n;
    }
    bool operator<=(const frac &a) const {
        return n*a.d <= d*a.n;
    }
    bool operator>=(const frac &a) const {
        return n*a.d >= d*a.n;
    }
    bool operator==(const ll &a) const {
        return n == a*d;
    }
    bool operator<(const ll &a) const {
        return n < a*d;
    }
    bool operator>(const ll &a) const {
        return n > a*d;
    }
    ll floor() const {
        if (n >= 0) return n/d;
        else return -((-n + d - 1)/d);
    }
    ll down() const {
        if (abs(n)%d == 0) return n/d-1;
        else return floor();
    }
    ll ceil() const {
        if (n >= 0) return (n + d - 1)/d;
        else return -((-n)/d);
    }
    ll up() const {
        if (abs(n)%d == 0) return n/d+1;
        else return ceil();
    }
    friend ostream& operator<<(ostream &os, const frac &f) {
        return os << f.n << "/" << f.d;
    }
};

struct point {
    frac x, y;
    point(ll xx=0, ll yy=0) : x(xx), y(yy) {}
    point(frac xx, frac yy) : x(xx), y(yy) {}
    point(pll p) : x(p.x), y(p.y) {}
    friend ostream& operator<<(ostream &os, const point &p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

int side(point a, point b, point c) {
    frac res = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
    if (res.n == 0) return 0;
    return res.n > 0 ? 1 : -1;
}

int N;
pll ps[55];

frac get(frac m, pll p, ll x) {
    return m*(x-p.x) + p.y;
}

frac get(frac m, point p, ll x) {
    return m*(frac(x)-p.x) + p.y;
}

int cnt_parallel(frac m, pll b, pll c) {
    m.red();
    repl(x,b.x,b.x+m.d) {
        frac y1 = get(m, b, x);
        frac y2 = get(m, c, x);
        if (y1 > y2) swap(y1, y2);
        ll l = y1.up();
        ll r = y2.down();
        if (l <= r) return -1;
    }
    return 0;
}

ll count(vt<point> &v) {
    sort(all(v), [](point &a, point &b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    ll l = v[0].x.ceil();
    ll r = v[2].x.floor();
    ll ans = 0;
    frac m0 = (v[1].y - v[0].y)/(v[1].x - v[0].x);
    frac m1 = (v[2].y - v[1].y)/(v[2].x - v[1].x);
    frac m2 = (v[2].y - v[0].y)/(v[2].x - v[0].x);
    m0.red();
    m1.red();
    m2.red();
    repl(x,l,r+1) {
        frac low = frac(1e9);
        frac high = frac(-1e9);
        if (v[0].x <= x && v[1].x >= x) {
            if (v[0].x == v[1].x) {
                continue;
            } else {
                frac y = get(m0, v[0], x);
                y.red();
                low = min(low, y);
                high = max(high, y);
            }
        }
        if (v[1].x <= x && v[2].x >= x) {
            if (v[1].x == v[2].x) {
                continue;
            } else {
                frac y = get(m1, v[1], x);
                y.red();
                low = min(low, y);
                high = max(high, y);
            }
        }
        if (v[0].x <= x && v[2].x >= x) {
            assert(v[0].x != v[2].x);
            frac y = get(m2, v[0], x);
            y.red();
            low = min(low, y);
            high = max(high, y);
        }
        ll ly = low.up();
        ll ry = high.down();
        if (ly <= ry) {
            ans += ry - ly + 1;
        }
    }
    return ans;
}

ll solve(pll a, pll b, pll c, pll d) {
    ll dx1 = b.x - a.x;
    ll dy1 = b.y - a.y;
    ll dx2 = c.x - d.x;
    ll dy2 = c.y - d.y;
    if (dx1*dy2 == dy1*dx2) {
        if (dx1 == 0) {
            swap(dx1, dy1);
            swap(b.x, b.y);
            swap(c.x, c.y);
        }
        int res = cnt_parallel(frac(dy1, dx1), b, c);
        return res;
    }
    // cout << "dx1: " << dx1 << ", dy1: " << dy1 << ", dx2: " << dx2 << ", dy2: " << dy2 << endl;
    frac t2 = frac(b.x*dy1 - c.x*dy1 + c.y*dx1 - b.y*dx1, dx2*dy1 - dy2*dx1);
    frac t1;
    if (dx1 != 0) {
        t1 = (t2*dx2 + c.x - b.x)/dx1;
    } else {
        t1 = (t2*dy2 + c.y - b.y)/dy1;
    }
    if (t1 < 0 || t2 < 0) return -1;
    frac x = t1*dx1 + b.x;
    frac y = t1*dy1 + b.y;
    x.red();
    y.red();
    vt<point> v = {point{b}, point{c}, point{x,y}};
    return count(v);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N;
    rep(i,0,N) cin >> ps[i];
    ll ans = 0;
    bool inf = 0;
    rep(i,0,N) {
        ll j = (i+1)%N;
        ll k = (i+2)%N;
        ll l = (i+3)%N;
        ll res = solve(ps[i], ps[j], ps[k], ps[l]);
        if (res == -1) {
            inf = 1;
            break;
        } else {
            ans += res;
        }
    }
    if (inf) {
        cout << "infinitely many" << endl;
    } else {
        cout << ans << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}