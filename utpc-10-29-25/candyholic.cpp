#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MOD 1000992299
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

int gcd(int a, int b) {
    if (a == -1) return b;
    if (b == -1) return a;
    return b ? gcd(b, a % b) : a;
}

struct GCDQueue {
    vt<pii> f, b; // val, gcd
    void push_back(int x) {
        int g = b.empty() ? x : gcd(b.back().y, x);
        b.pb({x, g});
    }
    void push_front(int x) {
        int g = f.empty() ? x : gcd(f.back().y, x);
        f.pb({x, g});
    }
    int get() {
        int gf = f.empty() ? -1 : f.back().y;
        int gb = b.empty() ? -1 : b.back().y;
        return gcd(gf, gb);
    }
    int pop() {
        if (f.empty()) {
            while (!b.empty()) {
                int x = b.back().x;
                b.pop_back();
                int g = f.empty() ? x : gcd(f.back().y, x);
                f.pb({x, g});
            }
        }
        int res = f.back().x;
        f.pop_back();
        return res;
    }
};

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

int N, K;
int a[200005], ls[200005];
int dp[200005][205], pre[200005][205];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> K;
    rep(i,1,N+1) {
        cin >> a[i];
        ls[i] = -1;
    }
    GCDQueue gq;
    int l = 1;
    rep(i,1,N+1) {
        gq.push_back(a[i]);
        int prev = -1;
        while (l < i && gq.get() == 1) {
            prev = gq.pop();
            l++;
        }
        if (prev != -1) {
            gq.push_front(prev);
            l--;
        }
        if (gq.get() == 1) {
            ls[i] = l-1;
        }
    }
    dp[0][0] = 1;
    pre[0][0] = 1;
    rep(i,1,N+1) {
        rep(j,1,K+1) {
            if (ls[i] != -1) {
                dp[i][j] = pre[ls[i]][j-1];
            }
            if (ls[i] != i-1) {
                dp[i][j] = ad(dp[i][j], dp[i-1][j-1]);
            }
            pre[i][j] = ad(pre[i-1][j], dp[i][j]);
        }
        pre[i][0] = ad(pre[i-1][0], dp[i][0]);
    }
    int ans = dp[N][K];
    cout << ans << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}