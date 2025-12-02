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

struct state {
    int x, y; ll d;
    bool operator<(const state &a) const {
        return d > a.d;
    }
};

int T, N, M;
ll dp[1005][1005];
char a[1005][1005];
const ll inf = LLONG_MAX;
const ll lim = 4'000'000'000'000'000'000LL;

int dx[] = {0,0,-1,1};
int dy[] = {-1,1,0,0};

bool in(int x, int y) {
    return x>=0 && x<N && y>=0 && y<M;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N >> M;
        int ex = -1, ey = -1;
        rep(i,0,N) {
            string s; cin >> s;
            rep(j,0,M) {
                a[i][j] = s[j];
                if (s[j] == '#') {
                    dp[i][j] = inf;
                } else if (s[j] == '.') {
                    dp[i][j] = inf;
                } else if (s[j] == 'A') {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = inf;
                    ex = i; ey = j;
                }
            }
        }
        priority_queue<state> pq;
        rep(i,0,N) {
            rep(j,0,M) {
                if (a[i][j] == '#') continue;
                rep(d,0,4) {
                    int ni = i+dx[d], nj = j+dy[d];
                    int ni2 = i+dx[d]*2, nj2 = j+dy[d]*2;
                    if (in(ni,nj) && in(ni2,nj2) && dp[ni][nj] < inf && dp[ni2][nj2] < inf) {
                        ll nd = dp[ni][nj] + dp[ni2][nj2];
                        if (nd > lim) continue;
                        if (ckmin(dp[i][j], nd)) {
                            pq.push({i, j, nd});
                        }
                    }
                }
            }
        }
        while (!pq.empty()) {
            state u = pq.top();
            pq.pop();
            if (u.d > dp[u.x][u.y]) continue;
            rep(d,0,4) {
                int ni_ = u.x-dx[d], nj_ = u.y-dy[d];
                int ni = u.x+dx[d], nj = u.y+dy[d];
                int ni2 = u.x+dx[d]*2, nj2 = u.y+dy[d]*2;
                if (in(ni,nj) && dp[ni][nj] < inf) {
                    ll nd = u.d + dp[ni][nj];
                    // cout << "Visiting: " << ni_ << ", " << nj_ << " with distance " << nd << endl;
                    if (in(ni_,nj_) && a[ni_][nj_] != '#' && ckmin(dp[ni_][nj_], nd)) {
                        if (dp[ni_][nj_] > lim) {
                            dp[ni_][nj_] = inf;
                        } else {
                            pq.push({ni_, nj_, dp[ni_][nj_]});
                        }
                    }
                    if (in(ni2,nj2) && a[ni2][nj2] != '#' && ckmin(dp[ni2][nj2], nd)) {
                        if (dp[ni2][nj2] > lim) {
                            dp[ni2][nj2] = inf;
                        } else {
                            pq.push({ni2, nj2, dp[ni2][nj2]});
                        }
                    }
                }
            }
        }
        ll ans = dp[ex][ey];
        cout << (ans == inf ? -1 : ans) << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}