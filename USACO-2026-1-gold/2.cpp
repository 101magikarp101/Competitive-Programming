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

int T, N;
int a[200005];

ll solve(int x) {
    ll ans = 0;
    rep(i,0,N) {
        if (a[i] == 0) {
            ans += abs(i - x);
            x++;
        }
    }
    return ans;
}

void solveeasy() {
    int z = 0;
    rep(i,0,N) {
        if (a[i] == 0) z++;
    }
    if (z == 0) {
        cout << 0 << endl;
        return;
    }
    int l = 0, r = N-z;
    while (r-l > 2) {
        int m1 = l + (r-l)/3;
        int m2 = r - (r-l)/3;
        ll r1 = solve(m1);
        ll r2 = solve(m2);
        if (r1 < r2) {
            r = m2;
        } else {
            l = m1;
        }
    }
    ll ans = 1e18;
    rep(i,l,r+1) {
        ans = min(ans, solve(i));
    }
    cout << ans << endl;
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
        set<int,greater<int>> s;
        map<int,int> mp;
        bool easy = 1;
        rep(i,0,N) {
            cin >> a[i];
            if (a[i] > 1) easy = 0;
            s.insert(a[i]);
        }
        if (easy) {
            solveeasy();
            continue;
        }
        each(x,s) {
            mp[x] = sz(mp)+1;
        }
        rep(i,0,N) {
            a[i] = mp[a[i]];
        }

        int n = sz(mp);
        vvi qs(n+1);
        rep(i,0,N) {
            int m = sz(qs[a[i]]);
            qs[a[i]].pb(i-m);
        }
        // cout << "qs:" << endl;
        // rep(i,1,n+1) {
        //     cout << i << ": ";
        //     each(x, qs[i]) {
        //         cout << x << " ";
        //     }
        //     cout << endl;
        // }
        vvi dp(n+1, vi(N,1e9));
        dp[0][0] = 0;
        int gap = N;
        rep(i,1,n+1) {
            int m = sz(qs[i]);
            gap -= m;
            vvi hm(N, vi(m+1, -1));
            rep(j,0,N) {
                int cur = 0;
                rep(k,0,m) cur += abs(qs[i][k]-j);
                rrep(k,m,0) {
                    assert(cur >= 0);
                    hm[j][k] = cur;
                    if (k) {
                        cur -= abs(qs[i][k-1]-j);
                        cur += abs(qs[i][k-1]-(j+gap));
                    }
                }
            }
            rep(j,0,N) {
                int best = 1e9;
                rep(k,0,m+1) {
                    if (j-k >= 0) ckmin(best, hm[j-k][k] + dp[i-1][j-k]);
                }
                dp[i][j] = best;
            }
        }
        cout << "dp:" << endl;
            rep(i,0,n+1) {
                rep(j,0,N) {
                    cout << dp[i][j] << " ";
                }
                cout << endl;
            }
        int ans = 1e9;
        rep(i,0,N) {
            ckmin(ans, dp[n][i]);
        }
        assert(ans != 1e9);
        assert(ans % 2 == 0);
        cout << ans/2 << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}