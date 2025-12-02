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

int N, M, n;
bool vis[55][55];
string s;
int sx, sy;
string ans;
vi path;
string dirs = "ENSW";
int rev[26];

int dx[] = {0,-1,1,0};
int dy[] = {1,0,0,-1};

int dtoi(char c) {
    return rev[c-'A'];
}

// 0 -> straight, 1 -> turn, -1 -> back
int rel(int a, int b) {
    if (a == b) return 0;
    else if (a+b == 3) return -1;
    else return 1;
}

bool in(int x, int y) {
    return 0<=x&&x<N&&0<=y&&y<M;
}

bool check(int i) {
    // cout << "path: ";
    // each(d, path) cout << d << " ";
    // cout << endl;
    int l = path[(i+n-2)%n];
    int m = path[(i+n-1)%n];
    int r = path[i];
    // cout << "Checking position " << i << " with moves " << l << ", " << m << ", " << r << " and s[i] = " << s[i] << endl;
    if (s[i] == 'B') {
        return l==0 && m==1 && r==0;
    } else if (s[i] == 'W') {
        return m==0 && l!=r;
    } else {
        return 1;
    }
}

int dist(int x, int y) {
    return abs(x - sx) + abs(y - sy);
}

// req: 0 -> straight, 1 -> turn, -1 -> any
bool dfs(int x, int y, int i, int req) {
    // cout << "At position (" << x << ", " << y << "), step " << i << ", req = " << req << endl;
    // cout << "board" << endl;
    // rep(a,0,N) {
    //     rep(b,0,M) {
    //         if (vis[a][b]) cout << "#";
    //         else cout << ".";
    //     }
    //     cout << endl;
    // }
    if (i == sz(s)) {

        if (x == sx && y == sy) {
            int dir = rel(dtoi(ans.back()), dtoi(ans[0]));
            // cout << "dir back to start: " << dir << endl;
            path.pb(dir);
            bool fin = check(0) && check(n-1) && check(1);
            path.pop_back();
            return fin;
        } else {
            return 0;
        }
    }
    if (s[(i+1)%n] == 'B') {
        if (req == 1) return 0;
        else req = 0;
    }
    if (req == 0 && s[i] == 'B') return 0;
    if (req == 1 && s[i] == 'W') return 0;
    // if (req == 1 && s[(i+1)%n] == 'B') return 0;
    
    int nreq = -1;
    // cout << "s: " << s << " i: " << i << endl;
    // cout << "s[i]: " << s[i] << endl;
    if (s[i] == 'B') {
        req = 1;
        nreq = 0;
    } else if (s[i] == 'W') {
        req = 0;
        if (sz(path)>0 && path.back() == 0) {
            nreq = 1;
        }
    }
    vis[x][y] = 1;
    rep(d,0,4) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (dist(nx,ny) > n-i-1) continue;
        // cout << "Trying direction " << dirs[d] << " to (" << nx << ", " << ny << ")" << endl;
        if (in(nx,ny) && (!vis[nx][ny] || (nx == sx && ny == sy && i == sz(s)-1))) {
            int dir = rel(dtoi(ans.back()), d);
            // cout << "ans: " << ans << endl;
            // cout << "dir: " << dir << ", req: " << req << endl;
            if (req == -1 || dir == req) {
                path.pb(dir);
                ans += dirs[d];
                bool res = dfs(nx, ny, i+1, nreq);
                if (res) return 1;
                ans.pop_back();
                path.pop_back();
            }
        }
    }
    vis[x][y] = 0;
    return 0;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    rep(i,0,4) {
        rev[dirs[i]-'A'] = i;
    }

    cin >> N >> N >> M;
    cin >> s;
    n = sz(s);
    // bool dup = 0;
    // rep(i,1,n) {
    //     if (s[i] == s[i-1] && s[i] == 'B') {
    //         dup = 1;
    //         break;
    //     }
    // }
    // if (dup) {
    //     cout << "impossible" << endl;
    //     return 0;
    // }
    cin >> sx >> sy;
    sx--; sy--;
    rep(d,0,4) {
        int nx = sx + dx[d];
        int ny = sy + dy[d];
        if (in(nx,ny)) {
            ans += dirs[d];
            vis[sx][sy] = 1;
            bool res = dfs(nx, ny, 1, s[0] == 'B' ? 0 : -1);
            if (res) break;
            vis[sx][sy] = 0;
            ans.pop_back();
        }
    }
    if (sz(ans) == 0) {
        cout << "impossible" << endl;
    } else {
        cout << ans << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}