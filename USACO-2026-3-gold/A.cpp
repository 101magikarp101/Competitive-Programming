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

template <class T> class BIT {
    // MUST be 1-indexed
public:
    vector<T> bit;
    int size;
    BIT(int n) {size = n; bit.resize(n + 1);}
    BIT(vi &v) {
        size = sz(v);
        bit.resize(size + 1);
        rep(i,1,size+1) {
            bit[i] += v[i-1];
            int j = i + (i & -i);
            if (j <= size) bit[j] += bit[i];
        }
    }
    void upd(int i, T x) {
        for (; i <= size; i += i & -i) bit[i] += x;
    }
    void set(int i, T x) {
        T cur = sum(i, i);
        upd(i, x - cur);
    }
    // returns sum of [1, i]
    T sum(int i) {
        if (i <= 0) return 0;
        T s = 0;
        for (; i > 0; i -= i & -i) s += bit[i];
        return s;
    }
    // returns sum of [l, r]
    T sum(int l, int r) {
        if (l > r) return 0;
        return sum(r) - (l>1 ? sum(l-1) : 0);
    }
};

int T, N;
int a[200005], b[200005], pos[200005];
ll pre[200005], dx[200005];

ll p[200005], inv[200005];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N;
        rep(i,0,N) {
            cin >> a[i];
            b[i] = a[i];
            a[i]--;
            a[i] -= i;
        }
        // left off by N
        BIT<ll> suml(2*N+1), sumr(2*N+1);
        BIT<int> cntl(2*N+1), cntr(2*N+1);
        rep(i,0,N) {
            cntl.upd(a[i]+N, 1);
            suml.upd(a[i]+N, a[i]);
        }
        rep(i,0,N) {
            int cll = cntl.sum(i+N);
            int clr = cntl.sum(i+N+1, 2*N);
            ll sll = suml.sum(i+N);
            ll slr = suml.sum(i+N+1, 2*N);
            ll resl = slr - (ll)clr*i + (ll)cll*i - sll;

            int crl = cntr.sum(i);
            int crr = cntr.sum(i+1, 2*N);
            ll srl = sumr.sum(i);
            ll srr = sumr.sum(i+1, 2*N);
            ll resr = srr - (ll)crr*i + (ll)crl*i - srl;

            p[i] = resl + resr;
            cntl.upd(a[N-1-i]+N, -1);
            suml.upd(a[N-1-i]+N, -a[N-1-i]);
            cntr.upd(a[N-1-i]+N, 1);
            sumr.upd(a[N-1-i]+N, a[N-1-i]+N);
        }
        // rep(i,0,N) {
        //     cout << p[i] << " ";
        // }
        // cout << endl;

        BIT<int> bit(N);
        ll cur = 0;
        rep(i,0,N) {
            cur += bit.sum(b[i]+1, N);
            bit.upd(b[i], 1);
        }
        rep(i,0,N) {
            inv[i] = cur;
            // cout << "inv[" << i << "] = " << inv[i] << endl;
            cur -= N-b[N-1-i];
            cur += b[N-1-i]-1;
        }
        vi ans;
        rep(i,0,N) {
            if (inv[i]*2 <= p[i]) ans.pb(i);
        }
        cout << sz(ans) << endl;
        rep(i,0,sz(ans)) {
            cout << ans[i];
            if (i != sz(ans)-1) cout << " ";
        }
        cout << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}