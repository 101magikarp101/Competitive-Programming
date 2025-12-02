// g++ -O2 -pipe -static -s -o sol sol.cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static inline void or_row_into_band(uint32_t *__restrict B,
                                    const bitset<512> *__restrict rowCols,
                                    int N, int K) {
    // rowCols[c] has 1s at columns where letter c appears in this row
    // Compose column masks by checking bit c at each column via bitset test.
    // This is branch-light and cache-friendly for N<=500, K<=26.
    for (int j = 0; j < N; ++j) {
        uint32_t m = 0;
        // Unroll over letters (K<=26)
        // Pull bits for column j across letters; this touches K/word memory locations.
        for (int c = 0; c < K; ++c) if (rowCols[c].test(j)) m |= (1u << c);
        B[j] |= m;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    // Build per-row, per-letter column bitsets (bitset helps row->band OR).
    static bitset<512> rowCols[505][26]; // rowCols[i][c][j] = 1 iff grid[i][j] == 'A'+c
    static uint32_t rowMask[505];        // OR over letters present in the row

    for (int i = 0; i < N; ++i) {
        string s; cin >> s;
        uint32_t rm = 0;
        for (int j = 0; j < N; ++j) {
            int c = s[j] - 'A';
            rowCols[i][c].set(j);
            rm |= (1u << c);
        }
        rowMask[i] = rm;
    }

    const uint32_t FULL = (K == 32 ? 0xFFFFFFFFu : ((1u << K) - 1u));

    // Global prune
    uint32_t global = 0;
    for (int i = 0; i < N; ++i) global |= rowMask[i];
    if ((global & FULL) != FULL) { cout << 0 << '\n'; return 0; }

    static uint32_t B[505];  // column masks for current band
    static int lastPos[32];  // last column where letter c appeared
    static uint8_t cnt[506]; // buckets over positions 0..N (fits in uint8_t since <=26)

    ll ans = 0;

    for (int top = 0; top < N; ++top) {
        memset(B, 0, N * sizeof(uint32_t));
        uint32_t bandMask = 0;

        for (int bot = top; bot < N; ++bot) {
            // extend band by one row
            bandMask |= rowMask[bot];
            // Column-wise composition using bitsets
            or_row_into_band(B, rowCols[bot], N, K);

            if ((bandMask & FULL) != FULL) continue; // can't form all K yet

            // Initialize per-band buckets
            for (int c = 0; c < K; ++c) lastPos[c] = -1;
            // cnt[0]=K, others 0
            memset(cnt, 0, (N+1) * sizeof(uint8_t));
            cnt[0] = (uint8_t)K;
            int minPos = 0;

            // Single pass over columns
            for (int r = 0; r < N; ++r) {
                uint32_t m = B[r] & FULL;
                // Update only letters present in this column
                while (m) {
                    uint32_t t = m & -m;
                    int c = __builtin_ctz(m);
                    m ^= t;
                    int old = lastPos[c] + 1; // 0..N
                    --cnt[old];
                    lastPos[c] = r;
                    ++cnt[r+1];
                }
                while (cnt[minPos] == 0) ++minPos; // amortized O(1)
                ans += minPos; // #valid left endpoints for rectangles ending at r
            }
        }
    }

    cout << ans << '\n';
    return 0;
}
