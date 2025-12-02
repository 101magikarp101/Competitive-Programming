#include <bits/stdc++.h>
using namespace std;

static const int qy[4] = {1, 0, -1, 0};
static const int qx[4] = {0, 1, 0, -1};
static const int MAXK = 240;

[[noreturn]] void wa(const string& msg) {
    // Match your original messages
    cerr << "WA: " << msg << "\n";
    exit(1); // non-zero = WA
}

[[noreturn]] void pe(const string& msg) {
    cerr << "output format error: " << msg << "\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 2; // usage error
    }

    ifstream inf(argv[1]);
    if (!inf) { cerr << "cannot open input file\n"; return 2; }
    ifstream ouf(argv[2]);
    if (!ouf) { cerr << "cannot open output file\n"; return 2; }

    int T;
    if (!(inf >> T)) pe("cannot read T");

    int WORST_RATIO = -1;
    int WORST_K = 0, WORST_N = 0;

    for (int t = 0; t < T; ++t) {
        int N, M;
        if (!(inf >> N >> M)) pe("cannot read N, M");

        vector<int> A(M), B(M);
        set<pair<int,int>> edges, edges_o;

        for (int i = 0; i < M; ++i) {
            if (!(inf >> A[i] >> B[i])) pe("cannot read edge");
            int u = A[i], v = B[i];
            if (u > v) edges.insert({u, v});
            else       edges.insert({v, u});
        }

        int K;
        if (!(ouf >> K)) pe("cannot read K");
        if (K < 1 || K > MAXK) wa("K out of bounds");

        // The original checker read K integers, each equal to K.
        for (int i = 0; i < K; ++i) {
            int tmp;
            if (!(ouf >> tmp)) pe("cannot read row size");
            if (tmp != K) wa("row size must equal K");
        }

        vector<vector<int>> R(K, vector<int>(K));
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                if (!(ouf >> R[i][j])) pe("cannot read grid value");
            }
        }

        // Validate adjacency-induced edges
        for (int l = 0; l < 4; ++l) {
            for (int i = 0; i < K; ++i) {
                for (int j = 0; j < K; ++j) {
                    int yy = i + qy[l];
                    int xx = j + qx[l];
                    if (yy < 0 || xx < 0 || yy >= K || xx >= K) continue;
                    int ai = R[i][j];
                    int bi = R[yy][xx];
                    if (ai == bi) continue;
                    pair<int,int> e = (ai > bi) ? make_pair(ai, bi) : make_pair(bi, ai);
                    if (!edges.count(e)) wa(" there are some extra edges in your graph ");
                    edges_o.insert(e);
                }
            }
        }

        if ((int)edges.size() != (int)edges_o.size()) {
            wa(" there are some missing edges in your graph ");
        }

        int TWICE_RATIO = 0;
        while (TWICE_RATIO * N < 2 * K) TWICE_RATIO++;
        if (TWICE_RATIO > WORST_RATIO) {
            WORST_RATIO = TWICE_RATIO;
            WORST_K = K; WORST_N = N;
        }
    }

    // Partial scoring path preserved (currently partial_score == 0)
    int partial_score = 0;
    if (partial_score == 0) {
        cout << "correct\n";
        return 0; // AC
    } else if (WORST_RATIO <= 4) {
        cout << "K <= 2N, K=" << WORST_K << ", N=" << WORST_N << "\n";
        return 0;
    } else if (WORST_RATIO <= 5) {
        cout << "K <= 2.5N, K=" << WORST_K << ", N=" << WORST_N << "\n";
        return 0;
    } else if (WORST_RATIO <= 6) {
        cout << "K <= 3N, K=" << WORST_K << ", N=" << WORST_N << "\n";
        return 0;
    } else if (WORST_RATIO <= 8) {
        cout << "K <= 4N, K=" << WORST_K << ", N=" << WORST_N << "\n";
        return 0;
    } else if (WORST_RATIO <= 12) {
        cout << "K <= 6N, K=" << WORST_K << ", N=" << WORST_N << "\n";
        return 0;
    } else {
        wa("K > 6N");
    }
}
