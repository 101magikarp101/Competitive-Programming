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

int ad(int a, int b) {
    a+=b;
    if (a>=MOD2) a-=MOD2;
    return a;
}

int sub(int a, int b) {
    a-=b;
    if (a<0) a+=MOD2;
    return a;
}

int mul(int a, int b) {
    return (int)((a*1LL*b)%MOD2);
}

int binpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b&1) res = mul(res, a);
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

int inv(int a) {
    return binpow(a, MOD2-2);
}

int di(int a, int b) {
    return mul(a, inv(b));
}

const int S = 2;
const int X[] = {31, 41};

int N, M;
char a[2005][2005];
int dp[2005][2005][S];
int ans[2005][2005];
int pre[4005][S], invs[4005][S];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    rep(i,0,S) {
        pre[0][i] = 1;
        rep(j,1,4001) pre[j][i] = mul(pre[j-1][i], X[i]);
        int in = inv(X[i]);
        invs[0][i] = 1;
        rep(j,1,4001) invs[j][i] = mul(invs[j-1][i], in);
    }

    int n, m;
    cin >> n >> m;
    vi tar(S, 0);
    rep(i,0,n) rep(j,0,m) {
        char c; cin >> c;
        // tar = ad(tar, mul(c-'a'+1, pre[i*m+j]));
        // cout << "pre[" << i*m+j << "] = " << pre[i*m+j][0] << ", " << pre[i*m+j][1] << endl;
        // tar = {ad(tar[0], mul(c-'a'+1, pre[i*m+j][0])), ad(tar[1], mul(c-'a'+1, pre[i*m+j][1]))};
        rep(k,0,S) tar[k] = ad(tar[k], mul(c-'a'+1, pre[i+j][k]));
    }
    // cout << "target: " << tar << endl;
    cin >> N >> M;
    rep(i,0,N) rep(j,0,M) {
        char c; cin >> c;
        a[i][j] = c;
        // dp[i][j] = mul(c-'a'+1, pre[i*m+j]);
        rep(k,0,S) dp[i][j][k] = mul(c-'a'+1, pre[i+j][k]);
    }
    rep(i,0,N) rep(j,0,M) {
        rep(k,0,S) {
            if (i) dp[i][j][k] = ad(dp[i][j][k], dp[i-1][j][k]);
            if (j) dp[i][j][k] = ad(dp[i][j][k], dp[i][j-1][k]);
            if (i && j) dp[i][j][k] = sub(dp[i][j][k], dp[i-1][j-1][k]);
        }
        // if (i) dp[i][j] = ad(dp[i][j], dp[i-1][j]);
        // if (j) dp[i][j] = ad(dp[i][j], dp[i][j-1]);
        // if (i && j) dp[i][j] = sub(dp[i][j], dp[i-1][j-1]);
    }
    rep(i,n-1,N) rep(j,m-1,M) {
        // int cur = dp[i][j];
        // if (i-n >= 0) cur = sub(cur, dp[i-n][j]);
        // if (j-m >= 0) cur = sub(cur, dp[i][j-m]);
        // if (i-n >= 0 && j-m >= 0) cur = ad(cur, dp[i-n][j-m]);
        vi cur(S, 0);
        rep(k,0,S) {
            cur[k] = dp[i][j][k];
            if (i-n >= 0) cur[k] = sub(cur[k], dp[i-n][j][k]);
            if (j-m >= 0) cur[k] = sub(cur[k], dp[i][j-m][k]);
            if (i-n >= 0 && j-m >= 0) cur[k] = ad(cur[k], dp[i-n][j-m][k]);
        }
        rep(k,0,S) cur[k] = mul(cur[k], invs[(i-n+1)+(j-m+1)][k]);
        if (cur == tar) {
            // cout << "found: " << i-n+1 << " " << j-m+1 << endl;
            ans[i-n+1][j-m+1]++;
            ans[i-n+1][j+1]--;
            ans[i+1][j-m+1]--;
            ans[i+1][j+1]++;
        }
    }
    rep(i,0,N) {
        rep(j,0,M) {
            if (i) ans[i][j] += ans[i-1][j];
            if (j) ans[i][j] += ans[i][j-1];
            if (i && j) ans[i][j] -= ans[i-1][j-1];
            cout << (ans[i][j]>0 ? a[i][j] : '.');
        }
        cout << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}