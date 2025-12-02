#include "obstacles.h"

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

template<class T> struct RMiQ {
  vvt<T> jmp;
  RMiQ(const vt<T>& V) : jmp(1, V) {
    for (int pw = 1, k = 1; pw * 2 <= sz(V); pw *= 2, ++k) {
      jmp.emplace_back(sz(V) - pw * 2 + 1);
      rep(j,0,sz(jmp[k]))
        jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
    }
  }
  RMiQ() = default;
  // 0-indexed, [a,b)
  T query(int a, int b) {
    assert(a < b); // or return inf if a == b
    int dep = 31 - __builtin_clz(b - a);
    return min(jmp[dep][a], jmp[dep][b - (1 << dep)]);
  }
};

template<class T> struct RMaQ {
  vvt<T> jmp;
  RMaQ(const vt<T>& V) : jmp(1, V) {
    for (int pw = 1, k = 1; pw * 2 <= sz(V); pw *= 2, ++k) {
      jmp.emplace_back(sz(V) - pw * 2 + 1);
      rep(j,0,sz(jmp[k]))
        jmp[k][j] = max(jmp[k - 1][j], jmp[k - 1][j + pw]);
    }
  }
  RMaQ() = default;
  // 0-indexed, [a,b)
  T query(int a, int b) {
    assert(a < b); // or return -inf if a == b
    int dep = 31 - __builtin_clz(b - a);
    return max(jmp[dep][a], jmp[dep][b - (1 << dep)]);
  }
};

int N, M;
bool inc = 1;
int a[200005], b[200005];
RMiQ<int> rmiq;
RMaQ<int> rmaq;

void initialize(vi c, vi d) {
  N = sz(c);
  M = sz(d);
  rep(i,0,N) {
    a[i] = c[i];
  }
  rep(i,0,M) {
    b[i] = d[i];
    // cout << "i: " << i << ", b[i]: " << b[i] << endl;
  }
  rep(i,1,N) {
    if (a[i] < a[i-1]) {
      inc = 0;
      break;
    }
  }
  rmiq = RMiQ<int>(d);
  rmaq = RMaQ<int>(d);
  return;
}

pii upd(int l, int r, int i) {
  int l1 = r, r1 = M-1;
  while (l1 < r1) {
    int m = (l1+r1+1)/2;
    if (rmaq.query(l,m+1) < a[i]) {
      l1 = m;
    } else {
      r1 = m-1;
    }
  }
  r = l1;
  l1 = 0, r1 = l;
  while (l1 < r1) {
    int m = (l1+r1)/2;
    if (rmaq.query(m,r+1) < a[i]) {
      r1 = m;
    } else {
      l1 = m+1;
    }
  }
  l = l1;
  return {l, r};
}

bool can_reach(int L, int R, int S, int D) {
  if (S > D) swap(S, D);
  if (inc) {
    return a[N-1] > rmaq.query(S,D+1);
  } else {
    int l = S, r = S, 
        l2 = D, r2 = D;
    // cout << "L: " << L << ", R: " << R << ", S: " << S << ", D: " << D << endl;
    bool ok = 0;
    rep(i,0,N) {
      auto res = upd(l, r, i);
      l = res.x;
      r = res.y;
      res = upd(l2, r2, i);
      l2 = res.x;
      r2 = res.y;
      // cout << "i: " << i << ", l: " << l << ", r: " << r << ", l2: " << l2 << ", r2: " << r2 << endl;
      if (r >= D || l2 <= S) {
        ok = 1;
        break;
      }
      if (a[i] <= rmiq.query(l,r+1) || a[i] <= rmiq.query(l2,r2+1)) {
        break;
      }
    }
    return ok;
  }
}
