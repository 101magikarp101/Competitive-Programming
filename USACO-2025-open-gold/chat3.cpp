#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

static inline long long sq_mod(long long x) {
    long long a = x % MOD;
    return (long long)((__int128)a * a % MOD);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long D;
    cin >> N >> D;

    int A, B;
    cin >> A >> B;

    vector<long long> m(N);
    for (int i = 0; i < N; i++) cin >> m[i];
    sort(m.begin(), m.end());

    long long K = (long long)A - (long long)B; // net + per day

    int split = N - A;                 // first 'split' are never touched
    vector<long long> active(m.begin() + split, m.end()); // size A

    // Sum of squares for untouched bottles
    long long ans = 0;
    for (int i = 0; i < split; i++) {
        ans += sq_mod(m[i]);
        if (ans >= MOD) ans -= MOD;
    }

    // Water-filling on active
    long long sum_active = 0;
    for (auto v : active) sum_active += v;

    long long S_final = sum_active + K * D; // target sum of active after D days

    long long lo = active.front();
    long long hi = active.back() + D; // inclusive upper search bound

    auto sum_clamped = [&](long long L) -> long long {
        __int128 s = 0;
        for (long long v : active) {
            if (v >= L) s += v;
            else if (v + D < L) s += (v + D);
            else s += L;
        }
        return (long long)s; // fits in 64-bit (<= ~2e14)
    };

    // Find largest L with sum_clamped(L) <= S_final
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (sum_clamped(mid) <= S_final) lo = mid;
        else hi = mid - 1;
    }
    long long L = lo;

    long long base_sum = sum_clamped(L);
    long long R = S_final - base_sum; // leftover units to bump L -> L+1

    // Compute sum of squares for active without explicitly constructing all w_i
    long long fixed_sq = 0;
    long long mid_count = 0; // count of indices with v < L < v+D (bumpable from L to L+1)

    for (long long v : active) {
        if (v >= L) {
            fixed_sq += sq_mod(v);
        } else if (v + D < L) {
            fixed_sq += sq_mod(v + D);
        } else {
            // v < L <= v+D
            if (L < v + D) {
                // bumpable: currently at L, can go to L+1
                mid_count++;
            } else {
                // exactly at cap v+D==L, stuck at L
                fixed_sq += sq_mod(L);
            }
        }
        fixed_sq %= MOD;
    }

    // R should be <= mid_count by maximality of L
    // Add (mid_count-R)*L^2 + R*(L+1)^2
    long long L2 = sq_mod(L);
    long long Lp12 = sq_mod(L + 1);

    long long add_mid = 0;
    add_mid = (add_mid + ( (mid_count - R) % MOD ) * L2) % MOD;
    add_mid = (add_mid + ( (R % MOD) * Lp12) % MOD) % MOD;

    long long active_sq = (fixed_sq + add_mid) % MOD;
    ans = (ans + active_sq) % MOD;

    cout << ans << "\n";
    return 0;
}
