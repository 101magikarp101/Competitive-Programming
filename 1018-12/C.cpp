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

int T, N;
ll a[1005][1005];
ll r[1005], c[1005];
bool rr[1005][4], cc[1005][4];
ll dpr[1005][2], dpc[1005][2];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N;
        rep(i,0,N) rep(j,0,N) cin >> a[i][j];
        rep(i,0,N) cin >> r[i];
        rep(i,0,N) cin >> c[i];
        rep(i,0,N) {
            rep(j,0,4) {
                rr[i][j] = 1;
                cc[i][j] = 1;
            }
            rep(j,0,2) {
                dpr[i][j] = 1e18;
                dpc[i][j] = 1e18;
            }
        }
        bool ok = 1;
        rep(i,0,N) {
            rep(j,1,N) {
                if (a[i][j-1] == a[i][j]) {
                    cc[j][0] = 0;
                    cc[j][3] = 0;
                } else if (a[i][j-1] == a[i][j]+1) {
                    cc[j][2] = 0;
                } else if (a[i][j-1]+1 == a[i][j]) {
                    cc[j][1] = 0;
                }
            }
        }
        rep(i,1,N) {
            rep(j,0,N) {
                if (a[i-1][j] == a[i][j]) {
                    rr[i][0] = 0;
                    rr[i][3] = 0;
                } else if (a[i-1][j] == a[i][j]+1) {
                    rr[i][2] = 0;
                } else if (a[i-1][j]+1 == a[i][j]) {
                    rr[i][1] = 0;
                }
            }
        }
        // cout << "rr:" << endl;
        // rep(i,0,N) {
        //     rep(j,0,4) {
        //         cout << rr[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << "cc:" << endl;
        // rep(i,0,N) {
        //     rep(j,0,4) {
        //         cout << cc[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        dpr[0][0] = 0;
        dpr[0][1] = r[0];
        rep(i,1,N) {
            rep(j,0,4) {
                if (!rr[i][j]) continue;
                int x = j%2, y = j/2;
                ckmin(dpr[i][y], dpr[i-1][x] + r[i]*y);
            }
        }
        dpc[0][0] = 0;
        dpc[0][1] = c[0];
        rep(i,1,N) {
            rep(j,0,4) {
                if (!cc[i][j]) continue;
                int x = j%2, y = j/2;
                // cout << "i: " << i << ", j: " << j << ", x: " << x << ", y: " << y << endl;
                // cout << "dpc[i-1][x]: " << dpc[i-1][x] << ", c[i]: " << c[i] << ", y: " << y << endl;
                ckmin(dpc[i][y], dpc[i-1][x] + c[i]*y);
            }
        }
        // cout << "dpc:" << endl;
        // rep(i,0,N) {
        //     rep(j,0,2) {
        //         cout << dpc[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        ll ans = 1e18;
        rep(i,0,2) rep(j,0,2) {
            ckmin(ans, dpr[N-1][i] + dpc[N-1][j]);
        }
        if (ans >= 1e18) {
            cout << -1 << endl;
        } else {
            cout << ans << endl;
        }
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}