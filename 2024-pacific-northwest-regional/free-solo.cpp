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

struct edge {
    int v;
    double w;
};

struct state {
    int u; double d;
    bool operator<(const state &a) const { return d > a.d; }
};


int N, M;
point a[35];
vt<point> vs;
vvt<edge> adj;
vt<double> dis;

const double EPS = 1e-9;
struct point {
    double x, y;
    point operator+(const point &a) const { return {x+a.x, y+a.y}; }
    point operator-(const point &a) const { return {x-a.x, y-a.y}; }
    point operator*(const double &a) const { return {x*a, y*a}; }
    point operator/(const double &a) const { return {x/a, y/a}; }
    void operator+=(const point &a) { x += a.x; y += a.y; }
    void operator-=(const point &a) { x -= a.x; y -= a.y; }
    void operator*=(const double &a) { x *= a; y *= a; }
    void operator/=(const double &a) { x /= a; y /= a; }
    bool operator==(const point &a) const { return abs(x - a.x) < EPS && abs(y - a.y) < EPS; }
    bool operator!=(const point &a) const { return !(*this == a); }
    friend ostream& operator<<(ostream &os, const point &p) {return os << "(" << p.x << ", " << p.y << ")";}
    friend istream& operator>>(istream &is, point &p) {return is >> p.x >> p.y;}
};
double dist(point p1, point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
double mag(point p) {
    return sqrt(p.x*p.x + p.y*p.y);
}
point norm(point p) {
    double d = mag(p);
    return {p.x/d, p.y/d};
}
double cross(point u, point v) {
    return u.x*v.y - u.y*v.x;
}
double dot(point u, point v) {
    return u.x*v.x + u.y*v.y;
}
// 2 circles
vt<point> intersect(point p1, point p2) {
    double d = dist(p1,p2)/2;
    if (d > 1.0) return {};
    point m = (p1 + p2) / 2.0;
    point v = p2-p1;
    v = {-v.y, v.x};
    v = norm(v);
    double h = sqrt(1.0 - d*d);
    point res1 = m + v*h;
    point res2 = m - v*h;
    return {res1, res2};
}
// projection of u onto v
point proj(point u, point v) {
    return v * (dot(u,v)/dot(v,v));
}
// line, circle
vt<point> intersect(point p1, point p2, point c) {
    point d = p2 - p1;
    double A = dot(d, d);
    if (A < EPS) return {};
    point ac = p1 - c;
    double B = 2.0 * dot(d, ac);
    double C = dot(ac, ac) - 1.0;
    double D = B*B - 4*A*C;
    if (D <= EPS) return {};
    double sq = sqrt(D);
    double t1 = (-B - sq) / (2*A);
    double t2 = (-B + sq) / (2*A);
    return {p1 + d * t1, p1 + d * t2};
}

bool check(point p1, point p2) {
    map<double,int> mp;
    point v = norm(p2-p1);
    double d = dist(p1,p2);
    // cout << "Checking " << p1 << " to " << p2 << endl;
    rep(i,0,N) {
        vt<point> tmp = intersect(p1, p2, a[i]);
        if (sz(tmp) == 0) continue;
        point v1 = tmp[0]-p1;
        point v2 = tmp[1]-p1;
        // cout << "  intersects circle at " << a[i] << " with points " << tmp[0] << " and " << tmp[1] << endl;
        double d1 = mag(v1);
        double d2 = mag(v2);
        point vv1 = norm(v1);
        point vv2 = norm(v2);
        if (vv1 != v) d1 = -d1;
        if (vv2 != v) d2 = -d2;
        if (d1 > d2) swap(d1,d2);
        if (d2 < 0 || d1 > d) continue;
        // cout << "       intersects circle at " << vs[i] << " with d1=" << d1 << ", d2=" << d2 << endl;
        d1 = max(d1, 0.0);
        mp[d1]++;
        if (d2 < d-EPS) mp[d2]--;
    }
    if (sz(mp) == 0) return false;
    int cnt = 0;
    each(i,mp) {
        cnt += i.se;
        if (cnt < 3) return false;
    }
    // cout << "  valid" << endl;
    return true;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N;
    a[0] = {0,0};
    a[1] = {-0.2,0};
    a[2] = {0.2,0};
    N += 3;
    rep(i,3,N) {
        cin >> a[i];
    }
    if (dist(a[0], a[N-1]) <= 1.0+EPS) {
        cout << 0 << endl;
        return 0;
    }
    vs.pb({0,0});
    rep(i,0,N) {
        rep(j,i+1,N) {
            vt<point> tmp = intersect(a[i], a[j]);
            // cout << "Intersecting " << a[i] << " and " << a[j] << endl;
            // cout << "  got " << sz(tmp) << " points" << endl;
            each(k,tmp) vs.pb(k);
        }
    }
    M = sz(vs);
    rep(i,0,M) {
        double d = dist(vs[i], a[N-1]);
        if (d < 1.0-EPS) continue;
        point v = vs[i]-a[N-1];
        v = norm(v);
        point p = a[N-1] + v;
        vs.pb(p);
        // cout << "Adding point " << p << endl;
    }
    M = sz(vs);
    adj = vvt<edge>(M);
    dis = vt<double>(M, 1e9);
    rep(i,0,M) {
        rep(j,i+1,M) {
            if (check(vs[i], vs[j])) {
                double d = dist(vs[i], vs[j]);
                adj[i].pb({j, d});
                adj[j].pb({i, d});
                // cout << "Edge: " << vs[i] << " to " << vs[j] << " with dist " << d << endl;
            }
        }
    }
    dis[0] = 0.0;
    priority_queue<state> pq;
    pq.push({0,0.0});
    while (!pq.empty()) {
        state s = pq.top(); pq.pop();
        if (s.d > dis[s.u]) continue;
        each(e, adj[s.u]) {
            if (ckmin(dis[e.v], dis[s.u] + e.w)) {
                pq.push({e.v, dis[e.v]});
            }
        }
    }
    double ans = 1e9;
    rep(i,0,M) {
        if (dist(vs[i], a[N-1]) <= 1.0+EPS) {
            ckmin(ans, dis[i]);
        }
    }
    if (ans > 1e8) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(10) << ans << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}