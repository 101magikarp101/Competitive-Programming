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

int T, N;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    cin >> T;
    while (T--) {
        cin >> N;
        vt<pii> a(N), b;
        int ma = -1;
        rep(i,0,N) {
            cin >> a[i].x;
            a[i].y = i;
            ckmax(ma, a[i].x);
        }
        vi ans(N,0);
        int l = 1, r = N;
        rep(j,1,ma+1) {
            vi hm, ids;
            rep(i,0,sz(a)) {
                if (a[i].x == j) {
                    hm.pb(a[i].y);
                    ids.pb(i);
                } else {
                    b.pb(a[i]);
                }
            }
            // cout << "hm: ";
            // each(i,hm) cout << i << " ";
            // cout << endl;
            // cout << "ids: ";
            // each(i,ids) cout << i << " ";
            // cout << endl;
            // cout << "sz(a): " << sz(a) << ", sz(b): " << sz(b) << endl;
            bool con = 1;
            rep(i,1,sz(ids)) {
                if (ids[i] != ids[i-1]+1) {
                    con = 0;
                    break;
                }
            }
            if (j%2) {
                if (con) {
                    if (ids[0] == 0) {
                        rep(i,0,sz(hm)) {
                            ans[hm[i]] = r--;
                        }
                    } else if (ids[sz(hm)-1] == sz(a)-1) {
                        rrep(i,sz(hm)-1,0) {
                            ans[hm[i]] = r--;
                        }
                    } else {
                        rep(i,0,sz(hm)) {
                            ans[hm[i]] = r--;
                        }
                    }
                } else {
                    int l1 = -1, r1 = -1;
                    ans[hm[0]] = r--;
                    rep(i,1,sz(ids)) {
                        if (ids[i] != ids[i-1]+1) {
                            l1 = i;
                            break;
                        } else {
                            ans[hm[i]] = r--;
                        }
                    }
                    ans[hm[sz(hm)-1]] = r--;
                    rrep(i,sz(ids)-2,0) {
                        if (ids[i] != ids[i+1]-1) {
                            r1 = i;
                            break;
                        } else {
                            ans[hm[i]] = r--;
                        }
                    }
                    rep(i,l1,r1+1) {
                        ans[hm[i]] = r--;
                    }
                }
            } else {
                if (con) {
                    if (ids[0] == 0) {
                        rep(i,0,sz(hm)) {
                            ans[hm[i]] = l++;
                        }
                    } else if (ids[sz(hm)-1] == sz(a)-1) {
                        rrep(i,sz(hm)-1,0) {
                            ans[hm[i]] = l++;
                        }
                    } else {
                        rep(i,0,sz(hm)) {
                            ans[hm[i]] = l++;
                        }
                    }
                } else {
                    int l1 = -1, r1 = -1;
                    ans[hm[0]] = l++;
                    rep(i,1,sz(ids)) {
                        if (ids[i] != ids[i-1]+1) {
                            l1 = i;
                            break;
                        } else {
                            ans[hm[i]] = l++;
                        }
                    }
                    ans[hm[sz(hm)-1]] = l++;
                    rrep(i,sz(ids)-2,0) {
                        if (ids[i] != ids[i+1]-1) {
                            r1 = i;
                            break;
                        } else {
                            ans[hm[i]] = l++;
                        }
                    }
                    rep(i,l1,r1+1) {
                        ans[hm[i]] = l++;
                    }
                }
            }
            // cout << "l: " << l << ", r: " << r << endl;
            a = b;
            b.clear();
            // cout << "ans: ";
            // each(i,ans) cout << i << " ";
            // cout << endl;
        }
        ans[a[0].y] = l;
        rep(i,0,N) {
            cout << ans[i] << " ";
        }
        cout << endl;
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}