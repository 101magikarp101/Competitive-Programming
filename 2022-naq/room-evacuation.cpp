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

template <class T = int> struct Dinic { // O(sqrt(V) * E) for unit capacity
    const static bool SCALING = false; // non-scaling = V^2E, Scaling=VElog(U) with higher constant, U is max capacity
    int N;
    int lim = 1;
    const T INF = numeric_limits<T>::max();
    struct edge {
        int to, rev;
        T cap, flow;
    };
    vi level, ptr;
    vvt<edge> adj;
    void init(int N) {
        this->N = N;
        level.resize(N); ptr.resize(N); adj.resize(N);
    }
    void addEdge(int a, int b, T cap, bool isDirected = true) {
        adj[a].push_back({b, sz(adj[b]), cap, 0});
        adj[b].push_back({a, sz(adj[a]) - 1, isDirected ? 0 : cap, 0});
    }
    bool bfs(int s, int t) {
        queue<int> q({s});
        fill(all(level), -1);
        level[s] = 0;
        while (!q.empty() && level[t] == -1) {
            int v = q.front();
            q.pop();
            for (auto e : adj[v]) {
                if (level[e.to] == -1 && e.flow < e.cap && (!SCALING || e.cap - e.flow >= lim)) {
                    q.push(e.to);
                    level[e.to] = level[v] + 1;
                }
            }
        }
        return level[t] != -1;
    }
    T dfs(int v, int t, T flow) {
        if (v == t || !flow)
            return flow;
        for (; ptr[v] < sz(adj[v]); ptr[v]++) {
            edge &e = adj[v][ptr[v]];
            if (level[e.to] != level[v] + 1)
                continue;
            if (T pushed = dfs(e.to, t, min(flow, e.cap - e.flow))) {
                e.flow += pushed;
                adj[e.to][e.rev].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
    T calc(int s, int t) {
        T flow = 0;
        for (lim = SCALING ? (1 << 30) : 1; lim > 0; lim >>= 1) {
            while (bfs(s, t)) {
                fill(all(ptr), 0);
                while (T pushed = dfs(s, t, INF))
                    flow += pushed;
            }
        }
        return flow;
    }
};

int N, M, T;
char a[25][25];

int dx[] = {-1,1,0,0,0};
int dy[] = {0,0,-1,1,0};

bool in(int x, int y) {
    return x>=0&&x<N&&y>=0&&y<M;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> M >> T;
    rep(i,0,N) rep(j,0,M) {
        cin >> a[i][j];
    }
    T++;
    Dinic<int> dinic;
    dinic.init(1+N*M*T+1);
    int s = 0, t = 1+N*M*T;
    rep(i,0,N) {
        rep(j,0,M) {
            if (a[i][j] == 'P') {
                dinic.addEdge(s, i*M+j+1, 1);
            }
            if (a[i][j] == 'E') {
                rep(k,0,T) {
                    dinic.addEdge(1+k*N*M + i*M+j, t, 1);
                }
            }
            if (a[i][j] == 'P' || a[i][j] == '.') {
                rep(k,0,T-1) {
                    rep(d,0,5) {
                        int nx = i+dx[d], ny = j+dy[d];
                        if (in(nx,ny) && a[nx][ny] != '#') {
                            dinic.addEdge(1+k*N*M + i*M+j, 1+(k+1)*N*M + nx*M+ny, 1);
                        }
                    }
                }
            }
        }
    }
    int ans = dinic.calc(s, t);
    cout << ans << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}