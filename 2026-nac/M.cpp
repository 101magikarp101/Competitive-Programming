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

struct maki {
    int s, x, c; // pos, num, cost
    bool operator<(const maki &a) const {
        if (s == a.s) return x > a.x;
        return s < a.s;
    }
};

int N, M, K, A, B;
vt<maki> ps[100005];

int solvetime(int t, vi &a, vi &b) {
    vt<maki> &v = ps[t];
    v.pb({A, 0, 0});
    v.pb({B, 0, 0});
    int n = sz(v);
    rep(i,0,n) {
        v.pb({v[i].s+N, v[i].x, v[i].c});
    }
    sort(all(v));
    int time = 0;
    ll cost = 0;
    int i = lower_bound(all(v), maki{A+N, 0, 0}) - v.begin();
    int j = lower_bound(all(v), maki{B+N, 0, 0}) - v.begin();
    deque<maki> adq, bdq, cdq;
    int cnta = 0, cntb = 0, cntc = 0;
    adq.pb(v[i]);
    bdq.pb(v[j]);
    int tot = 0;
    auto add = [&](maki m, deque<maki> &dp1, deque<maki> &dp2, int &cnt1, int &cnt2, bool hm) {
        if (hm) {
            maki nm = dp2.front();
            dp2.pop_front();
            cnt2 -= nm.x;
            cdq.pb(nm);
            cntc += nm.x;
        } else {
            dp1.pb(m);
            cnt1 += m.x;
        }
    };
    auto upda = [&]() {
        i--;
        add(v[i], adq, bdq, cnta, cntb, v[i].s <= B);
    };
    auto updb = [&]() {
        j--;
        add(v[j], bdq, adq, cntb, cnta, v[j].s <= A+N);
    };
    auto ok = [&]() {
        return cntc >= max(0, a[t]-cnta) + max(0, b[t]-cntb);
    };
    while (!ok()) {
        assert(i!=0 && j!=0);
        bool oka = v[i-1].s > A;
        bool okb = v[j-1].s > B;
        int dista = A+N - v[i-1].s;
        int distb = B+N - v[j-1].s;
        if (oka && okb) {
            if (dista == distb) {
                upda(); updb();
            } else if (dista < distb) {
                upda();
            } else {
                updb();
            }
        } else if (oka) {
            upda();
        } else if (okb) {
            updb();
        } else {
            break;
        }
    }
    time = max(A+N - v[i].s, B+N - v[j].s);
    // cout << "i: " << i << " j: " << j << " time: " << time << endl;
    if (!ok()) return -1;
    return time;
}

ll solvecost(int i, vi &a, vi &b, int time) {
    vt<maki> &v = ps[i];
    int n = sz(v);
    vt<maki> va, vb, vc;
    rep(i,0,n) {
        if (A+N-time <= v[i].s && v[i].s <= B || B+N-time <= v[i].s && v[i].s <= A+N) {
            vc.pb(v[i]);
        } else if (A+N-time <= v[i].s && v[i].s <= A+N) {
            va.pb(v[i]);
        } else if (B+N-time <= v[i].s && v[i].s <= B+N && v[i].s < A+N-time+N){
            vb.pb(v[i]);
        }
    }
    auto cmp = [&](maki m1, maki m2) {
        return m1.c < m2.c;
    };
    sort(all(va), cmp);
    sort(all(vb), cmp);
    sort(all(vc), cmp);
    ll cost = 0;
    int idxa = 0, idxb = 0, idxc = 0;
    int tota = 0;
    while (idxa < sz(va)) {
        maki &m = va[idxa];
        int use = min(m.x, a[i]-tota);
        tota += use;
        cost += (ll)use * m.c;
        m.x = use;
        if (tota >= a[i]) break;
        if (idxa == sz(va)-1) break;
        idxa++;
    }
    int totb = 0;
    while (idxb < sz(vb)) {
        maki &m = vb[idxb];
        int use = min(m.x, b[i]-totb);
        totb += use;
        cost += (ll)use * m.c;
        m.x = use;
        if (totb >= b[i]) break;
        if (idxb == sz(vb)-1) break;
        idxb++;
    }
    int totc = 0;
    while (idxc < sz(vc)) {
        maki &m = vc[idxc];
        int use = min(m.x, a[i]-tota + b[i]-totb - totc);
        totc += use;
        cost += (ll)use * m.c;
        m.x -= use;
        if (totc >= a[i]-tota + b[i]-totb) break;
        idxc++;
    }
    while ((idxa>=0 || idxb>=0) && idxc<sz(vc)) {
        int ca = idxa >= 0 && sz(va) > 0 ? va[idxa].c : INT_MIN;
        int cb = idxb >= 0 && sz(vb) > 0 ? vb[idxb].c : INT_MIN;
        if (max(ca,cb) <= vc[idxc].c) break;
        if (ca > cb) {
            int take = min(va[idxa].x, vc[idxc].x);
            va[idxa].x -= take;
            vc[idxc].x -= take;
            cost -= (ll)take * (ca - vc[idxc].c);
            if (va[idxa].x == 0) idxa--;
            if (vc[idxc].x == 0) idxc++;
        } else {
            int take = min(vb[idxb].x, vc[idxc].x);
            vb[idxb].x -= take;
            vc[idxc].x -= take;
            cost -= (ll)take * (cb - vc[idxc].c);
            if (vb[idxb].x == 0) idxb--;
            if (vc[idxc].x == 0) idxc++;
        }
    }
    return cost;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> M >> K >> A >> B;
    vi a(M), b(M);
    rep(i,0,M) cin >> a[i];
    rep(i,0,M) cin >> b[i];
    if (A > B) {
        swap(A, B);
        swap(a, b);
    }
    rep(i,0,K) {
        int s, t, x, c; cin >> s >> t >> x >> c;
        t--;
        ps[t].pb({s,x,c});
    }
    int time = 0;
    ll cost = 0;
    rep(i,0,M) {
        int t = solvetime(i, a, b);
        if (t == -1) {
            cout << "impossible" << endl;
            return 0;
        }
        time = max(time, t);
    }
    rep(i,0,M) {
        cost += solvecost(i, a, b, time);
    }
    cout << time << " " << cost << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}