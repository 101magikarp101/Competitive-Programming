#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using LL = long long;
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

std::pair<std::vector<int>, long long> transaction(long long M);

ll a[105];
int cnts[105];

pair<vi, ll> process(vi v, ll x, int i) {
    while (v.back() > i) {
        x -= a[v.back()];
        v.pop_back();
    }
    return {v, x};
}

void add(vi &v) {
    rep(i,0,sz(v)) {
        cnts[v[i]]++;
    }
}

void buy_souvenirs(int N, ll P0) {
    vt<pair<vi,ll>> ress;
    pair<vi,ll> tmp = transaction(P0-1);
    ress.pb({tmp.fi, P0-1-tmp.se});
    add(tmp.fi);
    rrep(i,N-1,1) {
        if (ress.empty()) break;
        auto [v, x] = process(ress.back().fi, ress.back().se, i);
        ress.pop_back();
        while (v[0] != i) {
            ress.pb({v, x});
            ll c = x/sz(v);
            if (sz(v) == 1) c--;
            auto res = transaction(c);
            add(res.fi);
            auto res2 = process(res.fi, c-res.se, i);
            v = res2.fi;
            x = res2.se;
        }
        a[i] = x;
    }
    rep(i,1,N) {
        rep(j,cnts[i],i) {
            auto res = transaction(a[i]);
        }
    }
}