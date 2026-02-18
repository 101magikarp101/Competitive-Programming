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
    int a, b, m;
    bool operator<(const state &s) const {
        return m > s.m;
    }
};

int A, B, X;
int dp[1005][1005];
state pre[1005][1005];
string sus[] = {"FILL A", "FILL B", "EMPTY A", "EMPTY B", "MOVE A B", "MOVE B A"};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> A >> B >> X;
    rep(i,0,A+1) {
        rep(j,0,B+1) {
            dp[i][j] = 2e9;
            pre[i][j] = {-1,-1,-1};
        }
    }
    priority_queue<state> pq;
    dp[0][0] = 0;
    pq.push({0,0,0});
    pii e = {-1,-1};
    while (!pq.empty()) {
        auto [x, y, m] = pq.top();
        // cerr << "Processing state: (" << x << ", " << y << ") with moves: " << m << endl;
        // cerr << "previous state: (" << pre[x][y].a << ", " << pre[x][y].b << ") with moves: " << pre[x][y].m << endl;
        pq.pop();
        if (x == X) {
            e = {x, y};
            break;
        }
        int d = A-x;
        if (m+d < dp[A][y]) { // FILL A
            dp[A][y] = m+d;
            pre[A][y] = {x, y, 0};
            pq.push({A, y, m+d});
        }
        d = B-y;
        if (m+d < dp[x][B]) { // FILL B
            dp[x][B] = m+d;
            pre[x][B] = {x, y, 1};
            pq.push({x, B, m+d});
        }
        d = x;
        if (m+d < dp[0][y]) { // EMPTY A
            dp[0][y] = m+d;
            pre[0][y] = {x, y, 2};
            pq.push({0, y, m+d});
        }
        d = y;
        if (m+d < dp[x][0]) { // EMPTY B
            dp[x][0] = m+d;
            pre[x][0] = {x, y, 3};
            pq.push({x, 0, m+d});
        }
        d = min(x, B-y);
        if (m+d < dp[x-d][y+d]) { // MOVE A B
            dp[x-d][y+d] = m+d;
            pre[x-d][y+d] = {x, y, 4};
            pq.push({x-d, y+d, m+d});
        }
        d = min(y, A-x);
        if (m+d < dp[x+d][y-d]) { // MOVE B A
            dp[x+d][y-d] = m+d;
            pre[x+d][y-d] = {x, y, 5};
            pq.push({x+d, y-d, m+d});
        }
    }
    if (e.x == -1) {
        cout << -1 << endl;
        return 0;
    }
    vt<state> ans;
    state s = pre[e.x][e.y];
    while (s.m != -1) {
        ans.pb(s);
        // cerr << "Backtracking state: (" << s.a << ", " << s.b << ") with moves: " << s.m << endl;
        s = pre[s.a][s.b];
    }
    reverse(all(ans));
    cout << sz(ans) << " " << dp[e.x][e.y] << endl;
    for (auto &s : ans) {
        cout << sus[s.m] << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}