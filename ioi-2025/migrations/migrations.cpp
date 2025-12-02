#include "migrations.h"
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

int N;
vi adj[10005];
pii diam = {0,0};
int len = 0;
vt<pii> ds;
vi hm;

vi b5(int x, int l) {
  vi res;
  while (x) {
    res.pb(x%5);
    x /= 5;
  }
  while (sz(res) < l) {
    res.pb(0);
  }
  return res;
}

int rev(vi &a, int l, int len) {
  int ans = 0;
  rrep(i,l+len-1,l) {
    ans = 5*ans + a[i];
  }
  return ans;
}

void upd(int i) {
  vi dist(N, -1);
  queue<int> q;
  dist[i] = 0;
  q.push(i);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : adj[u]) {
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push(v);
      }
    }
  }
  if (ckmax(len, dist[diam.x])) {
    diam.y = i;
  } else if (ckmax(len, dist[diam.y])) {
    diam.x = i;
  }
}

int send_message(int n, int i, int Pi) {
  N = n;
  adj[i].pb(Pi);
  adj[Pi].pb(i);
  upd(i);

  if (i == N-22) {
    vi v1 = b5(diam.x, 6);
    vi v2 = b5(diam.y, 6);
    each(x,v1) hm.pb(x);
    each(x,v2) hm.pb(x);
    ds.pb(diam);
  } else if (i == N-10) {
    int x = 0;
    if (diam.x != ds.back().x) {
      x = diam.x - (N-22);
    }
    int y = 0;
    if (diam.y != ds.back().y) {
      y = diam.y - (N-22);
    }
    vi v1 = b5(x, 2);
    vi v2 = b5(y, 2);
    each(x,v1) hm.pb(x);
    each(x,v2) hm.pb(x);

    ds.pb(diam);
  } else if (i == N-6) {
    int x = 0;
    if (diam.x != ds.back().x) {
      x = diam.x - (N-10);
    }
    int y = 0;
    if (diam.y != ds.back().y) {
      y = diam.y - (N-10);
    }
    hm.pb(x);
    hm.pb(y);

    ds.pb(diam);
  } else if (i == N-3) {
    int x = 0;
    if (diam.x != ds.back().x) {
      x = diam.x - (N-6);
    }
    hm.pb(x);

    ds.pb(diam);
  } else if (i == N-2) {
    int y = 0;
    if (diam.y != ds[sz(ds)-2].y) {
      y = diam.y - (N-6);
    }
    hm.pb(y);

    ds.pb(diam);
  } else if (i == N-1) {
    int x = 0;
    if (diam.x != ds[sz(ds)-2].x) {
      x = diam.x - (N-3);
    }
    int y = 0;
    if (diam.y != ds.back().y) {
      y = diam.y - (N-2);
    }
    if (y == 0) {
      hm.pb(x);
    } else {
      hm.pb(x+3);
    }

    ds.pb(diam);
  }
  if (i >= N-21) {
    int res = hm[i-(N-21)];
    return res;
  } else {
    return 0;
  }
}

std::pair<int, int> longest_path(vi a) {
  int n = sz(a);
  int x = -1, y = -1;
  // rep(i,n-21,n) {
  //   cout << a[i] << " ";
  // }
  // cout << endl;
  x = rev(a, n-21, 6);
  y = rev(a, n-15, 6);
  int z = rev(a, n-9, 2);
  if (z != 0) {
    x = n-22 + z;
  }
  z = rev(a, n-7, 2);
  if (z != 0) {
    y = n-22 + z;
  }
  z = rev(a, n-5, 1);
  if (z != 0) {
    x = n-10 + z;
  }
  z = rev(a, n-4, 1);
  if (z != 0) {
    y = n-10 + z;
  }
  z = rev(a, n-3, 1);
  if (z != 0) {
    x = n-6 + z;
  }
  z = rev(a, n-2, 1);
  if (z != 0) {
    y = n-6 + z;
  }
  z = rev(a, n-1, 1);
  if (z == 1) {
    x = n-2;
  } else if (z == 2) {
    x = n-1;
  } else if (z == 3) {
    y = n-1;
  } else if (z == 4) {
    x = n-2;
    y = n-1;
  }
  return {x, y};
}
