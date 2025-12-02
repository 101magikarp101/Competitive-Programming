// nest_robbing.cpp
// Variable-K Brute Force + Branch-and-Bound + Fuzzer for "Nest Robbing"

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------- Utilities ----------
static vector<int> compress_species(const vector<int>& s) {
    vector<int> xs = s, uniq = xs;
    sort(uniq.begin(), uniq.end());
    uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());
    vector<int> out(xs.size());
    for (size_t i = 0; i < xs.size(); ++i)
        out[i] = int(lower_bound(uniq.begin(), uniq.end(), xs[i]) - uniq.begin());
    return out;
}

// ---------- 1) Full brute force (for small n only) ----------
ll solve_bruteforce(int n, const vector<int>& s_in, const vector<int>& w, int K) {
    vector<int> s = compress_species(s_in);
    int S = 0; for (int x : s) S = max(S, x+1);

    ll best = 0;
    if (n <= 24) {
        uint64_t total = (n == 64 ? ~0ULL : (1ULL << n));
        for (uint64_t mask = 0; mask < total; ++mask) {
            if ((int)__builtin_popcountll(mask) > K) continue;        // quota
            if (mask & (mask << 1)) continue;                         // no adjacent

            vector<char> seen(S, 0);                                   // species unique
            bool ok = true;
            ll sum = 0;
            for (int i = 0; i < n; ++i) if (mask & (1ULL << i)) {
                if (seen[s[i]]) { ok = false; break; }
                seen[s[i]] = 1;
                sum += w[i];
            }
            if (ok) best = max(best, sum);
        }
    } else {
        // Exhaustive recursion (still brute, less convenient for large n; used only if n is big)
        vector<char> seen(S, 0);
        function<void(int,int,ll,bool)> dfs = [&](int i, int taken, ll cur, bool prevTaken){
            if (taken <= K) best = max(best, cur);
            if (i == n || taken == K) return;

            // skip i
            dfs(i+1, taken, cur, false);

            // take i
            if (!prevTaken && !seen[s[i]] && taken < K) {
                seen[s[i]] = 1;
                dfs(i+1, taken+1, cur + w[i], true);
                seen[s[i]] = 0;
            }
        };
        dfs(0,0,0,false);
    }
    return best;
}

// ---------- 2) Branch-and-Bound (fast, exact) ----------
ll solve_bnb(int n, const vector<int>& s_in, const vector<int>& w_in, int K) {
    vector<int> s = compress_species(s_in);
    vector<int> w = w_in;
    int S = 0; for (int x : s) S = max(S, x+1);

    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b){
        if (w[a] != w[b]) return w[a] > w[b];
        return a < b;
    });

    vector<unsigned char> avail(n, 1);
    vector<unsigned char> usedSpecies(S, 0);
    ll best = 0;

    // Greedy seed
    {
        auto A = avail;
        auto U = usedSpecies;
        ll gsum = 0;
        int taken = 0;
        for (int p = 0; p < n && taken < K; ++p) {
            int i = ord[p];
            if (!A[i] || U[s[i]]) continue;
            U[s[i]] = 1;
            A[i] = 0;
            if (i-1 >= 0) A[i-1] = 0;
            if (i+1 < n)  A[i+1] = 0;
            gsum += w[i];
            ++taken;
        }
        best = gsum;
    }

    auto upper_add = [&](int startPos, int remain)->ll {
        if (remain <= 0) return 0LL;
        ll sum = 0; int cnt = 0;
        for (int p = startPos; p < n && cnt < remain; ++p) {
            int i = ord[p];
            if (!avail[i]) continue;
            if (usedSpecies[s[i]]) continue; // still admissible
            sum += w[i];
            ++cnt;
        }
        return sum;
    };

    function<void(int,int,ll)> dfs = [&](int pos, int chosen, ll cur){
        if (cur > best) best = cur;
        if (chosen == K) return;

        int p = pos;
        while (p < n) {
            int i = ord[p];
            if (avail[i] && !usedSpecies[s[i]]) break;
            ++p;
        }
        if (p == n) return;

        ll ub = cur + upper_add(p, K - chosen);
        if (ub <= best) return;

        int i = ord[p];
        int sp = s[i];

        // take
        {
            vector<int> flipped;
            auto off = [&](int j){
                if (0 <= j && j < n && avail[j]) { avail[j] = 0; flipped.push_back(j); }
            };
            usedSpecies[sp] = 1;
            off(i);
            off(i-1);
            off(i+1);

            dfs(p+1, chosen+1, cur + w[i]);

            for (int j : flipped) avail[j] = 1;
            usedSpecies[sp] = 0;
        }
        // skip
        bool was = avail[i];
        if (was) avail[i] = 0;
        dfs(p+1, chosen, cur);
        if (was) avail[i] = 1;
    };

    dfs(0,0,0);
    return best;
}

// ---------- 3) Fuzzer with random K ----------
struct Case {
    int n, K;
    vector<int> s, w;
};

static Case random_case(mt19937_64& rng) {
    uniform_int_distribution<int> dn(1, 20);   // small n so brute force is feasible
    uniform_int_distribution<int> dw(1, 200);  // weights
    Case tc;
    tc.n = dn(rng);

    // species count between 1 and min(n, 15)
    int maxS = max(1, min(tc.n, 15));
    uniform_int_distribution<int> ds(1, maxS);

    tc.s.resize(tc.n);
    tc.w.resize(tc.n);
    for (int i = 0; i < tc.n; ++i) {
        tc.s[i] = ds(rng);
        tc.w[i] = dw(rng);
    }
    // Random K in [0, n] (cap at 16 to keep brute happy when n is large; n ≤ 20 here)
    uniform_int_distribution<int> dK(0, tc.n);
    tc.K = dK(rng);
    return tc;
}

static void print_case(const Case& tc) {
    cerr << "n = " << tc.n << ", K = " << tc.K << "\n";
    cerr << "s: ";
    for (int i = 0; i < tc.n; ++i) cerr << tc.s[i] << (i+1==tc.n?'\n':' ');
    cerr << "w: ";
    for (int i = 0; i < tc.n; ++i) cerr << tc.w[i] << (i+1==tc.n?'\n':' ');
}

static void run_checker() {
    mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    size_t trials = 0;
    for (;;) {
        ++trials;
        Case tc = random_case(rng);

        ll b1 = solve_bruteforce(tc.n, tc.s, tc.w, tc.K);
        ll b2 = solve_bnb(tc.n, tc.s, tc.w, tc.K);

        if (b1 != b2) {
            cerr << "COUNTEREXAMPLE FOUND after " << trials << " trials!\n";
            print_case(tc);
            cerr << "bruteforce = " << b1 << "\n";
            cerr << "bnb        = " << b2 << "\n";
            return;
        }
        if ((trials % 1000) == 0) {
            cerr << "Checked " << trials << " cases… still matching.\n";
        }
    }
}

// ---------- Main ----------
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Modes: default (bnb), "brute", "check"
    bool mode_brute = false, mode_check = false;
    int K = 12; // default

    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "brute") mode_brute = true;
        else if (a == "check") mode_check = true;
        else if (a.rfind("k=", 0) == 0 || a.rfind("K=", 0) == 0) {
            K = stoi(a.substr(2));
            if (K < 0) K = 0;
        }
    }

    if (mode_check) {
        run_checker();
        return 0;
    }

    int n;
    if (!(cin >> n)) return 0;
    vector<int> s(n), w(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    for (int i = 0; i < n; ++i) cin >> w[i];

    // Cap K at n
    K = min(K, n);

    if (mode_brute) {
        cout << solve_bruteforce(n, s, w, K) << '\n';
    } else {
        cout << solve_bnb(n, s, w, K) << '\n';
    }
    return 0;
}
