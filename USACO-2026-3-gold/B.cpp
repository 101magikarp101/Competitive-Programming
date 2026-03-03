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

int T, N, M, K, L;
vi adj[200005];
bool flo[200005], dest[200005];
int dist[200005];
int pre[200005], suf[200005];

vi vs[200005];
bool flod[200005];
int maxd = 0, maxf = -1;

void bfs(int u) {
    queue<int> q;
    q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        vs[dist[u]].pb(u);
        ckmax(maxd, dist[u]);
        if (flo[u]) {
            flod[dist[u]] = 1;
            ckmax(maxf, dist[u]);
        }
        each(v, adj[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    rep(d,0,maxd+1) {
        each(u, vs[d]) {
            each(v, adj[u]) {
                if (dist[v] > dist[u]) {
                    ckmax(pre[v], pre[u] + flo[v]);
                }
            }
        }
    }
}

void solverev() {
    rrep(d,maxd,0) {
        each(u, vs[d]) {
            if (dist[u] > maxf && dest[u]) {
                suf[u] = 0;
            } else if (dist[u] == maxf && dest[u] && flo[u]) {
                suf[u] = 1;
            }
            // cout << "u = " << u << ", pre[u] = " << pre[u] << ", suf[u] = " << suf[u] << endl;
            if (suf[u] == -1) continue;
            each(v, adj[u]) {
                if (dist[v] < dist[u]) {
                    ckmax(suf[v], suf[u] + flo[v]);
                }
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

    cin >> T;
    while (T--) {
        cin >> N >> M >> K >> L;
        maxd = 0;
        maxf = -1;
        rep(i,0,N+1) {
            adj[i].clear();
            vs[i].clear();
            pre[i] = 0;
            suf[i] = -1;
            flo[i] = 0;
            dest[i] = 0;
            dist[i] = -1;
            flod[i] = 0;
        }
        rep(i,0,K) {
            int x; cin >> x;
            flo[x] = 1;
        }
        rep(i,0,L) {
            int x; cin >> x;
            dest[x] = 1;
        }
        rep(i,0,M) {
            int u, v; cin >> u >> v;
            adj[u].pb(v);
            adj[v].pb(u);
        }
        dist[1] = 0;
        bfs(1);

        // cout << "dist: ";
        // rep(i,1,N+1) {
        //     cout << dist[i] << " ";
        // }
        // cout << endl;

        // bool imp = 0;
        // set<int> s;
        // rep(i,0,N) {
        //     if (flo[i] && s.count(i)) {
        //         imp = 1;
        //         break;
        //     }
        //     if (flo[i]) s.insert(i);
        // }
        // if (imp) {
        //     cout << string(N-1, '0') << endl;
        //     continue;
        // }

        solverev();

        // cout << "pre: ";
        // rep(i,1,N+1) {
        //     cout << pre[i] << " ";
        // }
        // cout << endl;
        // cout << "suf: ";
        // rep(i,1,N+1) {
        //     cout << suf[i] << " ";
        // }
        // cout << endl;

        vi ans(N+1,0);
        rep(i,2,N+1) {
            ans[i] = (pre[i] + suf[i] - flo[i]) >= K;
        }
        rep(i,2,N+1) {
            cout << (ans[i]?'1':'0');
        }
        cout << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}