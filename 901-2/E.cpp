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

int T;
const int p5[] = {1,5,25,125,625,3125,15625,78125,390625};
int hm[1<<16];
int dp[390625];

int get(int n, int x) {
    return (n >> (x * 2)) & 3;
}

int get5(int n, int x) {
    return n / p5[x] % 5;
}

int st(int n, int x, int v) {
    return (n & ~(3 << (x * 2))) | (v << (x * 2));
}

int op(int n, int x) {
    if (x==0) {
        n = st(n, 2, get(n, 0));
        n = st(n, 6, get(n, 4));
        return n;
    } else if (x==1) {
        n = st(n, 1, get(n, 3));
        n = st(n, 5, get(n, 7));
        return n;
    } else if (x==2) {
        int tmp = get(n, 2);
        n = st(n, 2, get(n, 3));
        n = st(n, 3, tmp);
        tmp = get(n, 6);
        n = st(n, 6, get(n, 7));
        n = st(n, 7, tmp);
        return n;
    } else {
        int tmp = get(n, 4);
        n = st(n, 4, get(n, 5));
        n = st(n, 5, tmp);
        tmp = get(n, 6);
        n = st(n, 6, get(n, 7));
        n = st(n, 7, tmp);
        return n;
    }
}

void solve() {
    queue<int> q;
    int s = 0;
    rep(i,0,8) {
        s |= (i%4) << (i*2);
    }
    q.push(s);
    hm[s] = 0;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        rep(i,0,4) {
            int y = op(x, i);
            if (hm[y] > hm[x] + 1) {
                hm[y] = hm[x] + 1;
                q.push(y);
            }
        }
    }
    rep(i,0,1<<16) {
        int x = 0;
        rep(j,0,8) {
            x += get(i,j)*p5[j];
        }
        dp[x] = hm[i];
    }
    rep(i,0,p5[8]) {
        rep(j,0,8) {
            if (get5(i, j) == 4) {
                rep(k,1,5) {
                    ckmin(dp[i], dp[i-k*p5[j]]);
                }
                break;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    rep(i,0,1<<16) hm[i] = 1e9;
    rep(i,0,p5[7]*5) dp[i] = 1e9;
    solve();

    cin >> T;
    while (T--) {
        int a, b, c, d, m; cin >> a >> b >> c >> d >> m;
        vi v(8,4);
        bool ok = 1;
        rep(i,0,30) {
            int x = (((m>>i)&1)<<2)|((a>>i)&1)<<1|((b>>i)&1);
            int y = ((c>>i)&1)<<1|((d>>i)&1);
            if (v[x] != 4 && v[x] != y) {
                ok = 0;
                break;
            }
            v[x] = y;
        }
        if (!ok) {
            cout << -1 << endl;
            continue;
        }
        int n = 0;
        rep(i,0,8) {
            n += v[i]*p5[i];
        }
        int res = dp[n];
        cout << (res == 1e9 ? -1 : res) << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}