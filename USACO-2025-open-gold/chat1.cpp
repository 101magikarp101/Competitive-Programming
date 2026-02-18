#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

long long modpow(long long a, unsigned long long e) {
    long long r = 1 % MOD;
    a %= MOD;
    while (e) {
        if (e & 1ULL) r = (long long)((__int128)r * a % MOD);
        a = (long long)((__int128)a * a % MOD);
        e >>= 1ULL;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long K;
    int N;
    unsigned long long L;
    cin >> K >> N >> L;
    string T;
    cin >> T;

    // Compute suffix O-counts at each M in T
    vector<int> sufO_for_M;
    sufO_for_M.reserve(N);

    int cntO = 0;
    for (int i = N - 1; i >= 0; --i) {
        if (T[i] == 'O') {
            cntO++;
        } else { // 'M'
            sufO_for_M.push_back(cntO); // O's strictly after this M within T
        }
    }

    long long m = (long long)sufO_for_M.size(); // #M in T
    long long o = (long long)cntO;              // #O in T

    // Guaranteed nonzero, so o == K*m
    // (If you want to be defensive, you could check and output 0 otherwise.)
    sort(sufO_for_M.begin(), sufO_for_M.end());

    // Build the per-copy product:
    // baseR is sufO_for_M expanded K times, sorted.
    // PM_onecopy = Π_{p=1..o} (baseR[p] - p + 1)
    long long PM_onecopy = 1;
    long long p = 1; // 1-index within baseR
    for (int v : sufO_for_M) {
        for (long long rep = 0; rep < K; rep++) {
            long long term = (long long)v - p + 1; // should be >= 1 for valid instances
            term %= MOD;
            if (term < 0) term += MOD;
            PM_onecopy = (long long)((__int128)PM_onecopy * term % MOD);
            p++;
        }
    }
    // p-1 should equal o

    // Numerator for whole S = (PM_onecopy)^L
    long long numerator = modpow(PM_onecopy, L);

    // Divide by (K!)^(mL)
    // Compute K! and its inverse mod MOD
    long long factK = 1;
    for (long long i = 2; i <= K; i++) {
        factK = (long long)((__int128)factK * (i % MOD) % MOD);
    }
    long long invFactK = modpow(factK, (unsigned long long)(MOD - 2));

    // Exponent can be huge: m*L up to ~1e24. Reduce mod (MOD-1) by Fermat (base != 0).
    long long modExp = (long long)((__int128)(m % (MOD - 1)) * (long long)(L % (MOD - 1)) % (MOD - 1));

    long long denom_inv = modpow(invFactK, (unsigned long long)modExp);

    long long ans = (long long)((__int128)numerator * denom_inv % MOD);
    cout << ans << "\n";
    return 0;
}
