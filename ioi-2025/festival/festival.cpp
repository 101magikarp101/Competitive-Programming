#include "festival.h"
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
  ll x, y; int i;
};

vi max_coupons(int A, vi a, vi t) {
  vi ans;
  vt<state> v;
  int n = sz(a);
  rep(i,0,n) {
    v.pb({a[i], t[i], i});
  }
  sort(all(v), [](const state &x, const state &y) {
    return x.x*x.y*y.y + y.x*y.y < y.x*x.y*y.y + x.x*x.y;
  });
  vvl dp(n+1, vl(n+1, LLONG_MIN));
  vvi prev(n+1, vi(n+1, -1));
  dp[0][0] = A;
  rep(i,1,n+1) {
    rep(j,0,i) {
      if (ckmax(dp[i][j], dp[i-1][j])) prev[i][j] = j;
      ll x = (dp[i-1][j]-v[i-1].x) * v[i-1].y;
      if (x < 0) continue;
      if (dp[i-1][j] < 0) continue;
      if (ckmax(dp[i][j+1], min(x, (ll)3e14))) prev[i][j+1] = j;
    }
  }
  cout << "dp: " << endl;
  rep(i,0,n+1) {
    rep(j,0,n+1) {
      cout << dp[i][j] << " ";
    }
    cout << endl;
  }
  cout << "prev: " << endl;
  rep(i,0,n+1) {
    rep(j,0,n+1) {
      cout << prev[i][j] << " ";
    }
    cout << endl;
  }
  rrep(i,n,1) {
    if (dp[n][i] >= 0) {
      int x = i;
      rrep(j,n,1) {
        if (prev[j][x] != x) {
          ans.pb(v[j-1].i);
          x = prev[j][x];
        }
      }
      break;
    }
  }
  reverse(all(ans));
  return ans;
}
