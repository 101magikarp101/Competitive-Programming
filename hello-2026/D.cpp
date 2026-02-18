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

int T, N;
set<int> adj[200005];
vi nodes[200005];
int col[200005];
int par[200005];
int ans = 0;

void dfs(int u, int p, int d) {
	int res = 1;
	nodes[d].pb(u);
	par[u] = p;
	each(v, adj[u]) {
		if (v == p) continue;
		dfs(v, u, d+1);
		res++;
	}
	ckmax(ans, res);
}

bool noedge(int u, int v) {
	return !adj[u].count(v);
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	#ifdef MAGIKARP
	auto start_time = chrono::high_resolution_clock::now();
	#endif

	cin >> T;
	while (T--) {
		cin >> N;
		// cout << "N=" << N << endl;
		rep(i,0,N+1) {
			nodes[i].clear();
			adj[i].clear();
			par[i] = -1;
			col[i] = -1;
		}
		ans = 0;
		rep(i,0,N-1) {
			int u, v; cin >> u >> v;
			adj[u].insert(v);
			adj[v].insert(u);
		}
		dfs(1, -1, 0);
		rep(i,0,N+1) {
			ckmax(ans, sz(nodes[i]));
		}
		col[1] = 0;
		set<int> cols;
		rep(i,0,ans) cols.insert(i);
		rep(d,1,N+1) {
			set<int> used;
			each(u, nodes[d]) {
				bool ok = 0;
				each(c, cols) {
					if (col[par[u]] != c) {
						col[u] = c;
						used.insert(c);
						cols.erase(c);
						ok = 1;
						break;
					}
				}
				if (!ok) {
					assert(sz(cols) == 1);
					int c = *cols.begin();
					each(v, nodes[d]) {
						if (col[par[v]] != c && col[v] != col[par[u]]) {
							used.insert(c);
							col[u] = col[v];
							col[v] = c;
							break;
						}
					}
				}
			}
			cols.insert(all(used));
			used.clear();
		}
		// cout << "col: ";
		// rep(i,1,N+1) cout << col[i] << " ";
		// cout << end;

		vvi fin(ans);
		rep(i,1,N+1) {
			fin[col[i]].pb(i);
		}
		int hm = 0;
		rep(i,0,ans) {
			hm += sz(fin[i]);
		}
		assert(hm == N);
		rep(i,0,ans) {
			rep(j,1,sz(fin[i])) {
				assert(noedge(fin[i][j-1], fin[i][j]));
			}
		}

		cout << ans << endl;
		each(v, fin) {
			cout << sz(v) << " ";
			each(x, v) cout << x << " ";
			cout << endl;
		}
	}

	#ifdef MAGIKARP
	auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
	cerr << "Time: " << duration/1000000.0 << "ms" << endl;
	#endif
	return 0;
}