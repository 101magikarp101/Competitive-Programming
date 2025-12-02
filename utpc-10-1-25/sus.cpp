#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// ---------- Brute force (minimax with memo) ----------
// State key helper
static string key_of(const vector<int>& a, bool ken_turn) {
    string s; s.reserve(a.size()*3 + 3);
    for (int v : a) { s += to_string(v); s.push_back(','); }
    s.push_back('|'); s.push_back(ken_turn ? 'K' : 'J');
    return s;
}

// Enumerate all compositions of k into 'parts' nonnegative integers.
// Calls cb(vec) for each composition.
static void compositions(int k, int parts, vector<int>& cur,
                         const function<void(const vector<int>&)>& cb) {
    if (parts == 1) {
        cur.push_back(k);
        cb(cur);
        cur.pop_back();
        return;
    }
    for (int x = 0; x <= k; ++x) {
        cur.push_back(x);
        compositions(k - x, parts - 1, cur, cb);
        cur.pop_back();
    }
}

// Minimax solver. Returns optimal final x from this state (current player tries to
// max if ken_turn, min otherwise). We *only* redistribute the k that was taken.
static unordered_map<string, long long> memo;

static long long solve_minimax(vector<int> a, bool ken_turn) {
    sort(a.begin(), a.end());
    string key = key_of(a, ken_turn);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    const int m = (int)a.size();
    if (m == 0) return 0;          // shouldn't really happen
    if (m == 1) return memo[key] = a[0];  // last pick; no redistribution

    long long best = ken_turn ? LLONG_MIN : LLONG_MAX;

    // Pick each distinct value once (avoid duplicate i with same a[i])
    for (int i = 0; i < m; ) {
        int k = a[i];

        // Build remaining vector
        vector<int> b; b.reserve(m-1);
        for (int j = 0; j < m; ++j) if (j != i) b.push_back(a[j]);

        // Distribute exactly k across (m-1) bins
        vector<int> cur;
        compositions(k, m - 1, cur, [&](const vector<int>& add) {
            vector<int> c = b;
            for (int t = 0; t < (int)c.size(); ++t) c[t] += add[t];
            sort(c.begin(), c.end());
            long long child = solve_minimax(c, !ken_turn);
            long long total = (long long)k + child;
            if (ken_turn) best = max(best, total);
            else          best = min(best, total);
        });

        // Skip equal values
        int v = a[i];
        while (i < m && a[i] == v) ++i;
    }

    return memo[key] = best;
}

// ---------- Your provided algorithm, wrapped as a callable ----------
static long long candidate_answer(vector<long long> a_ll) {
    int N = (int)a_ll.size();
    vector<long long> a = a_ll;
    sort(a.begin(), a.end());

    long long x = a[0];
    long long y = 0;
    long long cnt = 1;
    int l = 0, r = 0;
    int L = 0, R = N - 1;

    auto dleft = [&]() -> long long {
        cnt--;
        l++;
        L++;
        long long res = x;
        if (l > r) {
            r = l;
            cnt = 1;
            if (l <= R) x = a[l];
            else x = 0; // safeguard
            y = 0;
        } else {
            if (y == cnt) {
                x++;
                y = 0;
            }
        }
        return res;
    };
    auto dright = [&]() -> long long {
        if (R > r) {
            return a[R--];
        } else {
            R--;
            cnt--;
            r--;
            long long res = y ? x + 1 : x;
            if (y) y--;
            return res;
        }
    };
    function<void(long long)> add = [&](long long v) {
        long long lim = r == R ? LLONG_MAX : a[r + 1];
        if (lim == x) {
            r++;
            cnt++;
            add(v);
            return;
        }
        long long take = min(v, (long long)cnt - y);
        y += take;
        v -= take;
        if (y == cnt) {
            x++;
            y = 0;
        }
        long long d = lim - x;
        long long q = (cnt==0 ? 0 : v / cnt);
        long long rem = (cnt==0 ? 0 : v % cnt);
        if (q >= d) {
            x += d;
            v -= d * cnt;
            r++;
            cnt++;
            add(v);
        } else {
            x += q;
            y += rem;
            if (r < R && x + 1 == a[r + 1]) {
                r++;
                cnt++;
                y++;
            }
        }
    };

    long long ans = 0;
    for (int i = 0; i < N; ++i) {
        long long v;
        if (i % 2 == 0) v = dright();   // Ken
        else            v = dleft();    // Jynxzi
        if (i != N - 1) add(v);
        ans += v;
    }
    return ans;
}

// ---------- Testing harness ----------
static void print_vec(const vector<int>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) cout << " ";
        cout << v[i];
    }
    cout << "]";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1) Exhaustive search over small nondecreasing arrays first
    for (int maxVal = 1; maxVal <= 5; ++maxVal) {
        for (int n = 1; n <= 6; ++n) {
            // generate all nondecreasing arrays of length n with entries in [1..maxVal]
            vector<int> cur;
            function<bool(int,int)> gen = [&](int idx, int last) -> bool {
                if (idx == n) {
                    memo.clear();
                    long long brute = solve_minimax(cur, /*Ken starts*/true);
                    vector<long long> cur_ll(cur.begin(), cur.end());
                    long long cand  = candidate_answer(cur_ll);
                    if (brute != cand) {
                        cout << "Mismatch found!\n";
                        cout << "n=" << n << "  maxVal=" << maxVal << "\n";
                        cout << "a = "; print_vec(cur); cout << "\n";
                        cout << "brute = " << brute << "\n";
                        cout << "candidate = " << cand << "\n";
                        return true; // stop
                    }
                    return false;
                }
                for (int v = last; v <= maxVal; ++v) {
                    cur.push_back(v);
                    if (gen(idx + 1, v)) return true;
                    cur.pop_back();
                }
                return false;
            };
            if (gen(0, 1)) return 0; // stop on first mismatch
        }
    }

    // 2) Random fuzzing (continues until mismatch)
    std::mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dn(1, 10);   // small n for brute
    uniform_int_distribution<int> dv(1, 6);   // small values for brute

    for (long long iters = 1;; ++iters) {
        int n = dn(rng);
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = dv(rng);

        memo.clear();
        long long brute = solve_minimax(a, true);
        vector<long long> a_ll(a.begin(), a.end());
        long long cand  = candidate_answer(a_ll);

        if (brute != cand) {
            cout << "Mismatch found after " << iters << " random cases!\n";
            cout << "n=" << n << "\n";
            cout << "a = "; print_vec(a); cout << "\n";
            cout << "brute = " << brute << "\n";
            cout << "candidate = " << cand << "\n";
            // break;
        }

        if (iters % 1000 == 0) {
            cerr << "Checked " << iters << " random cases...\n";
        }
    }

    return 0;
}
