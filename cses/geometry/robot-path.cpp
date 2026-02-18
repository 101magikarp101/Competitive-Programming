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

int N;
ll pre[100005];

template<typename T> class SegTree {
private:
    const T DEFAULT = INT_MAX;
    vt<mset<int>> tr2;
    vt<T> tr;
    int len;
public:
    SegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
        tr2.assign(len*2, mset<int>());
    }
    SegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        tr2.assign(len*2, mset<int>());
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = min(tr[i*2], tr[i*2+1]);
    }
    void add(int i, T v) {
        tr2[i].insert(v);
        int j = i + len;
        tr[j] = *tr2[i].begin();
        i = j;
        for (i >>= 1; i > 0; i >>= 1) {
            tr[i] = min(tr[i*2], tr[i*2+1]);
        }
    }
    void remove(int i, T v) {
        tr2[i].erase(tr2[i].find(v));
        int j = i + len;
        if (tr2[i].empty()) {
            tr[j] = DEFAULT;
        } else {
            tr[j] = *tr2[i].begin();
        }
        i = j;
        for (i >>= 1; i > 0; i >>= 1) {
            tr[i] = min(tr[i*2], tr[i*2+1]);
        }
    }
    // query range [l, r)
    T query(int l, int r) {
        l += len;
        r += len;
        T res = DEFAULT;
        while (l < r) {
            if (l & 1) ckmin(res, tr[l++]);
            if (r & 1) ckmin(res, tr[--r]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

struct line {
    ll v, l, r; int i; ll x;
    bool operator<(const line &a) const {
        return v < a.v;
    }
};

struct state {
    ll r, v; int i;
    bool operator<(const state &a) const {
        return r > a.r;
    }
};

pll dir(char c) {
    if (c == 'U') return {0, 1};
    else if (c == 'D') return {0, -1};
    else if (c == 'L') return {-1, 0};
    else return {1, 0};
}

vl comp(vt<line> &ls) {
    set<ll> s;
    each(l,ls) {
        s.insert(l.v);
    }
    vl vals(all(s));
    map<ll,int> m;
    each(i,s) {
        m[i] = sz(m);
    }
    each(l,ls) {
        l.v = m[l.v];
    }
    return vals;
}

int le(vl &v, ll x) {
    return int(lower_bound(all(v), x) - v.begin());
}

int ge(vl &v, ll x) {
    return int(upper_bound(all(v), x) - 1 - v.begin());
}

ll solve(vt<line> xs, vt<line> ys) {
    sort(all(xs));
    sort(all(ys), [](line &a, line &b) {
        return a.l < b.l;
    });
    vl yvals = comp(ys);
    int M = sz(yvals);
    SegTree<int> seg(M);
    priority_queue<state> pq;
    int idx = 1e9;
    int j = 0;
    ll l1 = -1, r1 = -1, x1 = -1, z = -1;
    each(x,xs) {
        while (j < sz(ys) && ys[j].l <= x.v) {
            line y = ys[j];
            seg.add(y.v, y.i);
            pq.push({y.r, y.v, y.i});
            j++;
        }
        while (!pq.empty() && pq.top().r < x.v) {
            state s = pq.top(); pq.pop();
            seg.remove(s.v, s.i);
        }
        int l = le(yvals, x.l);
        int r = ge(yvals, x.r);
        int res = seg.query(l, r+1);
        // cout << "checking x line i=" << x.i << " v=" << x.v << " l=" << x.l << " r=" << x.r << " mapped to [" << l << ", " << r << "] got res i=" << res << endl;
        if (res < x.i-1) {
            if (ckmin(idx, x.i)) {
                l1 = l;
                r1 = r;
                x1 = x.x;
                z = x.v;
            }
        }
    }
    if (idx >= 1e9) {
        return LLONG_MAX;
    }
    ll ans = (idx?pre[idx-1]:0);
    ll ad = 1e18;
    each(y,ys) {
        if (y.i >= idx-1) continue;
        if (y.v < l1 || y.v > r1) continue;
        if (y.l <= z && z <= y.r) {
            ckmin(ad, abs(x1 - yvals[y.v]));
        }
    }
    return ans + ad;
}

bool intersect(pii a, pii b) {
    return max(a.x, b.x) <= min(a.y, b.y);
}

ll solve2(vt<line> &v) {
    map<ll,set<pll>> m;
    sort(all(v), [](line &a, line &b) {
        return a.i < b.i;
    });
    ll ans = LLONG_MAX;
    ll ad = LLONG_MAX;
    int i = 0;
    each(li,v) {
        while (i < sz(v) && v[i].i < li.i-1) {
            m[v[i].v].insert({v[i].l, v[i].r});
            i++;
        }
        auto it = m[li.v].upper_bound({li.l, LLONG_MAX});
        if (it != m[li.v].begin() && prev(it)->y >= li.l) {
            it--;
        }
        bool ok = 0;
        while (it != m[li.v].end() && it->x <= li.r) {
            ok = 1;
            pll cur = *it;
            ckmin(ad, min(abs(li.x - it->x), abs(li.x - cur.y)));
            it++;
        }
        if (ok) {
            ans = (li.i?pre[li.i-1]:0) + ad;
            break;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N;
    pll prev = {0,0};
    pll cur = {0,0};
    vt<line> xs, ys;
    ll ans = LLONG_MAX;
    ll tot = 0;
    rep(i,0,N) {
        char c; int d; cin >> c >> d;
        pll move = dir(c);
        if (move + prev == pll{0,0}) {
            ans = tot;
            break;
        }
        auto [x1, y1] = cur;
        auto [x2, y2] = cur + move * d;
        if (move.x == 0) {
            xs.pb({x1, min(y1, y2), max(y1, y2), i, y1});
        } else {
            ys.pb({y1, min(x1, x2), max(x1, x2), i, x1});
        }
        cur += move * d;
        tot += d;
        prev = move;
        pre[i] = (i?pre[i-1]:0) + d;
    }
    ans = tot;
    ckmin(ans, solve(xs, ys));
    ckmin(ans, solve(ys, xs));
    ckmin(ans, solve2(xs));
    ckmin(ans, solve2(ys));
    cout << ans << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}