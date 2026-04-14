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

struct boss {
	ll h, d;
};

struct hero {
	ll h, d, c;
};

int N, M, C;
boss a[1000005];
hero b[5005];
ll pre[1000005];

pll dp[5005][5005];

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	#ifdef MAGIKARP
	auto start_time = chrono::high_resolution_clock::now();
	#endif

	cin >> N >> M >> C;
	rep(i,0,N) {
		cin >> a[i].h >> a[i].d;
		pre[i] = a[i].h + (i?pre[i-1]:0);
	}
	rep(i,1,M+1) {
		cin >> b[i].h >> b[i].d >> b[i].c;
	}
	rep(i,0,M+1) {
		rep(j,0,C+1) {
			dp[i][j] = {0, 0};
		}
	}
	rep(i,1,M+1) {
		rep(j,0,C+1) {
			dp[i][j] = dp[i-1][j];
			if (j >= b[i].c) {
				auto [bo, d] = dp[i-1][j-b[i].c];
				// cout << "i: " << i << " j: " << j << " bo: " << bo << " d: " << d << endl;
				int bb = bo%N;
				ll dmg = (a[bb].d?(b[i].h-1)/a[bb].d * b[i].d: 1e12);
				d += dmg;
				if (d >= a[bb].h) {
					bo++;
					d = 0;
				}
				// cout << "dmg: " << dmg << " rem: " << rem << " bo: " << bo << " d: " << d << endl;
				ckmax(dp[i][j], {bo, d});
			}
		}
	}
	pll res = {0, 0};
	rep(i,0,C+1) {
		ckmax(res, dp[M][i]);
	}
	ll ans = res.x/N * pre[N-1];
	if (res.x > 0 && res.x%N != 0) {
		ans += pre[(res.x-1)%N];
	}
	cout << res.y << " " << ans+res.y << endl;

	#ifdef MAGIKARP
	auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
	cerr << "Time: " << duration/1000000.0 << "ms" << endl;
	#endif
	return 0;
}