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

struct frac{
    ll n, d;
    frac() {}
    frac(ll n, ll d): n(n), d(d) {}
    bool operator<(const frac &f) const {
        return n*f.d < f.n*d;
    }
    frac operator+(const frac &f) const {
        frac res = {n*f.d + f.n*d, d*f.d};
        res.red();
        return res;
    }
    frac operator*(const frac &f) const {
        frac res = {n*f.n, d*f.d};
        res.red();
        return res;
    }
    frac operator*(const ll &a) const {
        frac res = {n*a, d};
        res.red();
        return res;
    }
    void red() {
        ll g = gcd(n, d);
        n /= g;
        d /= g;
    }
};

struct parab {
    ll a, b, c;
    frac f;
    int i;
    bool operator<(const parab &p) const {
        if (a == p.a) {
            return f < p.f;
        } else {
            return a < p.a;
        }
    }
};

int T, N;
parab a[3005];
int pre[3005], suf[3005];
int ans[3005];

bool good(parab &p1, parab &p2) {
    ll A = p2.a-p1.a;
    ll B = p2.b-p1.b;
    ll C = p2.c-p1.c;
    if (A == 0 && B == 0) {
        return C != 0;
    } else if (A == 0) {
        return 0;
    }
    return B*B < 4*A*C;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N;
        rep(i,0,N) {
            cin >> a[i].a >> a[i].b >> a[i].c;
            frac f = {-a[i].b, 2*a[i].a};
            f = f*f*a[i].a + f*a[i].b + frac(a[i].c, 1);
            a[i].f = f;
            a[i].i = i;
        }
        sort(a,a+N);
        suf[N-1] = 1;
        rrep(i,N-2,0) {
            suf[i] = 1;
            rep(j,i+1,N) {
                if (good(a[i], a[j])) {
                    suf[i] = max(suf[i], suf[j]+1);
                }
            }
        }
        pre[0] = 1;
        rep(i,1,N) {
            pre[i] = 1;
            rep(j,0,i) {
                if (good(a[j], a[i])) {
                    pre[i] = max(pre[i], pre[j]+1);
                }
            }
        }
        // cout << "a: ";
        // rep(i,0,N) {
        //     cout << a[i].a << " " << a[i].b << " " << a[i].c << " " << a[i].i << endl;
        // }
        // cout << "pre: ";
        // rep(i,0,N) {
        //     cout << pre[i] << " ";
        // }
        // cout << endl;
        // cout << "suf: ";
        // rep(i,0,N) {
        //     cout << suf[i] << " ";
        // }
        // cout << endl;
        rep(i,0,N) {
            ans[a[i].i] = pre[i]+suf[i]-1;
        }
        rep(i,0,N) {
            cout << ans[i] << " ";
        }
        cout << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}