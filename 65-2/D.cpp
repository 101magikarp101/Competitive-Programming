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
    int i, j, jok;
};

int N, M;
string a[20][20];

string ranks = "A23456789TJQK";
string suits = "CDHS";
set<string> cards;

vt<pii> check() {
    vvt<bool> ans(N, vt<bool>(M, false));
    rep(i,0,N-2) {
        rep(j,0,M-2) {
            set<char> ss, rs;
            rep(x,0,3) {
                rep(y,0,3) {
                    ss.insert(a[i+x][j+y][1]);
                    rs.insert(a[i+x][j+y][0]);
                }
            }
            if (sz(ss) == 1 || sz(rs) == 9) {
                ans[i][j] = 1;
            }
        }
    }
    vt<pii> ret;
    rep(i,0,N-2) {
        rep(j,0,M-2) {
            rep(k,0,N-2) {
                rep(l,0,M-2) {
                    if (abs(i-k) <= 2 && abs(j-l) <= 2) continue;
                    if (ans[i][j] && ans[k][l]) {
                        ret.pb({i+1, j+1});
                        ret.pb({k+1, l+1});
                        return ret;
                    }
                }
            }
        }
    }
    return ret;
}

void print(vt<pii> &v) {
    cout << "Put the first square to (" << v[0].x << ", " << v[0].y << ")." << endl;
    cout << "Put the second square to (" << v[1].x << ", " << v[1].y << ")." << endl;
}

bool isjok(string &s) {
    return s[0] == 'J' && (s[1] == '1' || s[1] == '2');
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    #ifdef MAGIKARP
    auto start_time = chrono::high_resolution_clock::now();
    #endif

    each(r, ranks) {
        each(s, suits) {
            cards.insert(string(1, r) + s);
        }
    }
    cards.insert("J1");
    cards.insert("J2");

    cin >> N >> M;
    vt<state> js;
    rep(i,0,N) {
        rep(j,0,M) {
            cin >> a[i][j];
            cards.erase(a[i][j]);
            if (isjok(a[i][j])) {
                js.pb({i, j, a[i][j][1] - '0'});
            }
        }
    }
    vt<string> cs(all(cards));
    if (sz(js) == 0) {
        vt<pii> ans = check();
        if (sz(ans) == 0) {
            cout << "No solution." << endl;
        } else {
            cout << "Solution exists." << endl;
            cout << "There are no jokers." << endl;
            print(ans);
        }
    } else if (sz(js) == 1) {
        auto [x, y, jok] = js[0];
        vt<pii> ans;
        string card;
        each(c, cs) {
            if (isjok(c)) continue;
            string s = a[x][y];
            a[x][y] = c;
            vt<pii> res = check();
            if (sz(res)) {
                ans = res;
                card = c;
                break;
            }
            a[x][y] = s;
        }
        if (sz(ans) == 0) {
            cout << "No solution." << endl;
        } else {
            cout << "Solution exists." << endl;
            cout << "Replace J" << jok << " with " << card << "." << endl;
            print(ans);
        }
    } else if (sz(js) == 2) {
        auto [x1, y1, jok1] = js[0];
        auto [x2, y2, jok2] = js[1];
        vt<pii> ans;
        string card1, card2;
        each(c1, cs) {
            each(c2, cs) {
                if (c1 == c2) continue;
                string s1 = a[x1][y1];
                string s2 = a[x2][y2];
                a[x1][y1] = c1;
                a[x2][y2] = c2;
                vt<pii> res = check();
                if (sz(res)) {
                    ans = res;
                    card1 = c1;
                    card2 = c2;
                    break;
                }
                a[x1][y1] = s1;
                a[x2][y2] = s2;
            }
            if (sz(ans)) break;
        }
        if (jok1 == 2) {
            swap(jok1, jok2);
            swap(card1, card2);
        }
        if (sz(ans) == 0) {
            cout << "No solution." << endl;
        } else {
            cout << "Solution exists." << endl;
            cout << "Replace J" << jok1 << " with " << card1 << " and J" << jok2 << " with " << card2 << "." << endl;
            print(ans);
        }
    }

    #ifdef MAGIKARP
    auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_time).count();
    cerr << "Time: " << duration/1000000.0 << "ms" << endl;
    #endif
    return 0;
}