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
    ll l, r, h;
    bool operator<(const state &a) const {
        return l < a.l;
    }
};

int N;
set<state> s;

bool add1(ll h, ll x) {
    state tmp = {x,0,0};
    auto it = s.upper_bound(tmp);
    it = prev(it);
    state o = {it->l,it->r,it->h};
    s.erase(it);
    if (o.l != x) {
        s.insert({o.l,x-1,o.h});
    }
    if (o.r != x) {
        s.insert({x+1,o.r,o.h});
    }
    h += o.h;
    auto [it2, b] = s.insert({x,x,h});
    if (it2 != s.begin()) {
        auto p = prev(it2);
        if (p->h == h) {
            s.erase(it2);
            s.erase(p);
            it2 = s.insert({p->l, it2->r, h}).fi;
        }
    }
    if (next(it2) != s.end()) {
        auto p = next(it2);
        if (p->h == h) {
            s.erase(it2);
            s.erase(p);
            s.insert({it2->l, p->r, h});
        }
    }
    return 1;
}

bool add2(ll l, ll r) {
    state tmp = {l,0,0};
    auto it = s.upper_bound(tmp);
    it = prev(it);
    state o = {it->l,it->r,it->h};
    if (l < o.l || r > o.r) return 0;
    s.erase(it);
    if (o.l != l) {
        s.insert({o.l,l-1,o.h});
    }
    if (o.r != r) {
        s.insert({r+1,o.r,o.h});
    }
    ll h = o.h + 1;
    auto [it2, b] = s.insert({l,r,h});
    if (it2 != s.begin()) {
        auto p = prev(it2);
        if (p->h == h) {
            s.erase(it2);
            s.erase(p);
            it2 = s.insert({p->l, it2->r, h}).fi;
        }
    }
    if (next(it2) != s.end()) {
        auto p = next(it2);
        if (p->h == h) {
            s.erase(it2);
            s.erase(p);
            s.insert({it2->l, p->r, h});
        }
    }
    return 1;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> N;
    s.insert({1,(ll)2e18,0});
    rep(i,0,N) {
        char c; ll l, p; cin >> c >> l >> p;
        bool ans;
        if (c == '|') {
            ans = add1(l, p);
        } else {
            ans = add2(p, p+l-1);
        }
        cout << (ans?'S':'U');
    }
    cout << endl;

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}