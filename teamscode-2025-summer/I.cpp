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

template <class T> class MaxSegTree {
private:
    const T DEFAULT = INT_MIN;
    vt<T> tr;
    int len;
public:
    MaxSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
    }
    MaxSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = max(tr[i*2], tr[i*2+1]);
    }
    void set(int i, T v) {
        i += len;
        tr[i] = v;
        for (i >>= 1; i > 0; i >>= 1) {
            tr[i] = max(tr[i*2], tr[i*2+1]);
        }
    }
    // query range [l, r)
    T query(int l, int r) {
        l += len;
        r += len;
        T res = DEFAULT;
        while (l < r) {
            if (l & 1) ckmax(res, tr[l++]);
            if (r & 1) ckmax(res, tr[--r]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

template <class T> class MinSegTree {
private:
    const T DEFAULT = INT_MAX;
    vt<T> tr;
    int len;
public:
    MinSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
    }
    MinSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = min(tr[i*2], tr[i*2+1]);
    }
    void set(int i, T v) {
        i += len;
        tr[i] = v;
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

template <class T> class LzSegTree {
  private:
    const T DEFAULT = INT_MAX;
    vt<T> tr;
    vt<T> lz;
    int len;
    void push(int n) {
        if (lz[n] == 0) return;
        lz[2*n] += lz[n];
        lz[2*n+1] += lz[n];
        tr[2*n] += lz[n];
        tr[2*n+1] += lz[n];
        lz[n] = 0;
    }
    void _set(int n, int s, int e, int i, T v) {
        if (s == e) {
            tr[n] = v;
            return;
        }
        int m = (s + e) / 2;
        push(n);
        if (i <= m) {
            _set(n*2, s, m, i, v);
        } else {
            _set(n*2+1, m+1, e, i, v);
        }
        tr[n] = min(tr[n*2], tr[n*2+1]);
    }
    void _range_add(int n, int s, int e, int l, int r, T val) {
        if (s > r || e < l) return;
        if (l <= s && e <= r) {
            tr[n] += val;
            lz[n] += val;
            return;
        }
        int m = (s + e) / 2;
        push(n);
        _range_add(n*2, s, m, l, r, val);
        _range_add(n*2+1, m+1, e, l, r, val);
        tr[n] = min(tr[n*2], tr[n*2+1]);
    }
    T _query(int n, int s, int e, int l, int r) {
        if (s > r || e < l) return DEFAULT;
        if (l <= s && e <= r) return tr[n];
        int m = (s + e) / 2;
        push(n);
        T left = _query(n*2, s, m, l, r);
        T right = _query(n*2+1, m+1, e, l, r);
        return min(left, right);
    }
    T _get(int n, int s, int e, int i) {
        if (s == e) return tr[n];
        push(n);
        int m = (s + e) / 2;
        if (i <= m) {
            return _get(n*2, s, m, i);
        } else {
            return _get(n*2+1, m+1, e, i);
        }
    }
  public:
    LzSegTree(vt<T> &a) {
        len = 1;
        while (len < sz(a)) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, DEFAULT);
        rep(i,0,sz(a)) tr[i+len] = a[i];
        rrep(i,len-1,1) tr[i] = tr[i*2] + tr[i*2+1];
    }
    LzSegTree(int n) {
        len = 1;
        while (len < n) len *= 2;
        tr.assign(len*2, DEFAULT);
        lz.assign(len*2, 0);
    }
    void set(int i, T v) {
        _set(1, 0, len-1, i, v);
    }
    void range_add(int l, int r, T val) {
        _range_add(1, 0, len-1, l, r, val);
    }
    int get(int i) {
        return _get(1, 0, len-1, i);
    }
    // query range [l, r]
    T query(int l, int r) {
        return _query(1, 0, len-1, l, r);
    }
    void print() {
        rep(i,0,len) {
            cout << get(i) << " ";
        }
        cout << endl;
    }
};

struct query {
    int r, i;
};

int N, Q;
int a[200005], rig[200005];
vt<query> qs[200005];
MaxSegTree<int> stl(200005);
int ans[200005];

void pre() {
    map<int,int> m;
    rep(i,0,N) {
        int x = -1;
        if (m.count(a[i])) {
            x = m[a[i]];
        }
        stl.set(i, x);
        m[a[i]] = i;
    }
    vi v(N);
    set<int> s;
    rep(i,0,N) s.insert(a[i]);
    map<int,int> m2;
    each(x, s) {
        m2[x] = sz(m2);
    }
    rep(i,0,N) {
        v[i] = m2[a[i]];
    }
    int M = sz(s);
    MinSegTree<int> str2(M+1);
    rep(i,0,M+1) {
        str2.set(i, N);
    }
    rrepl(i,N-1,0) {
        int res = str2.query(v[i]+1, M+1);
        rig[i] = res;
        str2.set(v[i], i);
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N >> Q;
    rep(i,0,N) cin >> a[i];
    pre();
    rep(i,0,Q) {
        int l, r; cin >> l >> r;
        l--; r--;
        int l1 = l, r1 = r;
        while (r1 > l1) {
            int m = (l1+r1+1)/2;
            if (stl.query(l, m+1) < l) {
                l1 = m;
            } else {
                r1 = m-1;
            }
        }
        r = l1;
        qs[l].pb({r, i});
    }
    LzSegTree<int> st(N);
    mset<int,greater<int>> ms;
    int r = 0;
    rep(i,0,N) {
        if (i) {
            ms.erase(ms.find(a[i-1]));
            st.set(i-1, INT_MAX);
            int R = rig[i-1];
            int diff = a[i-1]-a[i];
            if (diff > 0) {
                diff--;
                int l1 = i, r1 = rig[i];
                while (r1 <= R) {
                    st.range_add(l1, r1-1, -diff);
                    if (r1 == N) break;
                    diff = a[i-1]-a[r1]-1;
                    l1 = r1;
                    r1 = rig[l1];
                }
            }
            if (R <= r-1) {
                st.range_add(R, r-1, 1);
            }
        }
        while (r < N && !ms.count(a[r])) {
            ms.insert(a[r]);
            st.set(r, *ms.begin()-sz(ms));
            r++;
        }
        each(q,qs[i]) {
            int res = st.query(i, q.r);
            ans[q.i] = res;
        }
    }
    rep(i,0,Q) {
        cout << ans[i] << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}