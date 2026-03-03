#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct FastScanner {
    static constexpr size_t BUFSIZE = 1 << 20;
    int idx = 0, size = 0;
    char buf[BUFSIZE];

    inline char read() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }

    template <class T>
    bool readInt(T &out) {
        char c;
        do {
            c = read();
            if (!c) return false;
        } while (c <= ' ');

        bool neg = false;
        if (c == '-') { neg = true; c = read(); }
        T val = 0;
        while (c > ' ') {
            val = val * 10 + (c - '0');
            c = read();
        }
        out = neg ? -val : val;
        return true;
    }
};

static inline string toString128(i128 v) {
    if (v == 0) return "0";
    bool neg = v < 0;
    if (neg) v = -v;
    string s;
    while (v > 0) {
        int digit = (int)(v % 10);
        s.push_back(char('0' + digit));
        v /= 10;
    }
    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}

struct CenterKey {
    int t; // 0 up, 1 down
    int x;
    int y;
    bool operator==(CenterKey const& o) const {
        return t == o.t && x == o.x && y == o.y;
    }
};

struct CenterKeyHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    size_t operator()(CenterKey const& k) const {
        uint64_t x = (uint64_t)(uint32_t)k.x;
        uint64_t y = (uint64_t)(uint32_t)k.y;
        uint64_t t = (uint64_t)k.t;
        uint64_t h = (t << 62) ^ (x * 1000003ULL) ^ (y * 10007ULL);
        return (size_t)splitmix64(h);
    }
};

struct CenterData {
    int64 w = 0;
    int64 a[3] = {0,0,0};
};

struct CenterInfo {
    int x, y;
    int s;
    int64 w;
    int64 a[3];
};

static inline uint64_t packXY(int x, int y) {
    uint64_t ux = (uint64_t)((uint32_t)x ^ 0x80000000u);
    uint64_t uy = (uint64_t)((uint32_t)y ^ 0x80000000u);
    return (ux << 32) | uy;
}

// sum_{i<j} w_i*w_j*|v_i-v_j|
static i128 weightedAbsWithin(vector<pair<int,int64>> vw) {
    sort(vw.begin(), vw.end(), [](auto &A, auto &B){ return A.first < B.first; });
    i128 prefW = 0, prefWV = 0, total = 0;
    for (auto &p : vw) {
        i128 v = p.first;
        i128 w = p.second;
        total += w * (v * prefW - prefWV);
        prefW += w;
        prefWV += w * v;
    }
    return total;
}

// sum_{a in A, b in B} w_a*w_b*|a-b|
static i128 weightedAbsBetween(vector<pair<int,int64>> A, vector<pair<int,int64>> B) {
    sort(B.begin(), B.end(), [](auto &L, auto &R){ return L.first < R.first; });
    int m = (int)B.size();
    vector<int> bv(m);
    vector<i128> prefW(m+1, 0), prefWV(m+1, 0);
    for (int i=0;i<m;i++){
        bv[i] = B[i].first;
        prefW[i+1] = prefW[i] + (i128)B[i].second;
        prefWV[i+1] = prefWV[i] + (i128)B[i].second * (i128)B[i].first;
    }
    i128 totalW = prefW[m], totalWV = prefWV[m];

    i128 ans = 0;
    for (auto &p : A) {
        int v = p.first;
        i128 w = p.second;
        int pos = (int)(lower_bound(bv.begin(), bv.end(), v) - bv.begin());
        i128 Wl = prefW[pos], WVl = prefWV[pos];
        i128 Wr = totalW - Wl, WVr = totalWV - WVl;
        ans += w * ( (i128)v * Wl - WVl + (WVr - (i128)v * Wr) );
    }
    return ans;
}

static i128 hexSumWithin(const vector<CenterInfo> &L) {
    if (L.size() <= 1) return 0;
    vector<pair<int,int64>> vx, vy, vs;
    vx.reserve(L.size()); vy.reserve(L.size()); vs.reserve(L.size());
    for (auto &c : L) {
        vx.push_back({c.x, c.w});
        vy.push_back({c.y, c.w});
        vs.push_back({c.s, c.w});
    }
    i128 ax = weightedAbsWithin(vx);
    i128 ay = weightedAbsWithin(vy);
    i128 as = weightedAbsWithin(vs);
    return (ax + ay + as) / 2;
}

static i128 hexSumBetween(const vector<CenterInfo> &A, const vector<CenterInfo> &B) {
    if (A.empty() || B.empty()) return 0;
    vector<pair<int,int64>> Ax, Ay, As, Bx, By, Bs;
    Ax.reserve(A.size()); Ay.reserve(A.size()); As.reserve(A.size());
    Bx.reserve(B.size()); By.reserve(B.size()); Bs.reserve(B.size());
    for (auto &c : A) {
        Ax.push_back({c.x, c.w});
        Ay.push_back({c.y, c.w});
        As.push_back({c.s, c.w});
    }
    for (auto &c : B) {
        Bx.push_back({c.x, c.w});
        By.push_back({c.y, c.w});
        Bs.push_back({c.s, c.w});
    }
    i128 ax = weightedAbsBetween(Ax, Bx);
    i128 ay = weightedAbsBetween(Ay, By);
    i128 as = weightedAbsBetween(As, Bs);
    return (ax + ay + as) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    int T;
    if (!fs.readInt(T)) return 0;

    while (T--) {
        int N;
        fs.readInt(N);

        unordered_map<CenterKey, CenterData, CenterKeyHash> mp;
        mp.reserve((size_t)N * 2);

        auto addPoint = [&](int x, int y, int z) {
            int triType, dp, dq, side;
            switch (z) {
                case 0:  triType=0; dp=0;  dq=0;  side=0; break;
                case 1:  triType=0; dp=0;  dq=0;  side=1; break;
                case 2:  triType=1; dp=-1; dq=+1; side=2; break;
                case 3:  triType=1; dp=-1; dq=+1; side=1; break;
                case 4:  triType=0; dp=-1; dq=0;  side=2; break;
                case 5:  triType=0; dp=-1; dq=0;  side=0; break;
                case 6:  triType=1; dp=-1; dq=0;  side=0; break;
                case 7:  triType=1; dp=-1; dq=0;  side=2; break;
                case 8:  triType=0; dp=0;  dq=-1; side=1; break;
                case 9:  triType=0; dp=0;  dq=-1; side=2; break;
                case 10: triType=1; dp=0;  dq=0;  side=1; break;
                case 11: triType=1; dp=0;  dq=0;  side=0; break;
                default: triType=0; dp=0; dq=0; side=0; break;
            }
            int p = x + dp;
            int q = y + dq;
            int X = -p - q;
            int Y = p;

            CenterKey key{triType, X, Y};
            auto &d = mp[key];
            d.w++;
            d.a[side]++;
        };

        for (int i = 0; i < N; i++) {
            int x,y,z;
            fs.readInt(x); fs.readInt(y); fs.readInt(z);
            addPoint(x,y,z);
        }

        vector<CenterInfo> up, down;
        up.reserve(mp.size());
        down.reserve(mp.size());

        i128 sameCenterSum = 0;
        i128 withinPairs = 0;
        int64 totalRegions = 0;

        for (auto &kv : mp) {
            const CenterKey &k = kv.first;
            const CenterData &d = kv.second;

            CenterInfo c;
            c.x = k.x; c.y = k.y; c.s = k.x + k.y;
            c.w = d.w;
            c.a[0]=d.a[0]; c.a[1]=d.a[1]; c.a[2]=d.a[2];

            totalRegions += c.w;

            i128 wp = (i128)c.w * (c.w - 1) / 2;
            withinPairs += wp;
            i128 zero = (i128)c.a[0]*(c.a[0]-1)/2 + (i128)c.a[1]*(c.a[1]-1)/2 + (i128)c.a[2]*(c.a[2]-1)/2;
            sameCenterSum += wp - zero;

            if (k.t == 0) up.push_back(c);
            else down.push_back(c);
        }

        i128 totalPairs = (i128)totalRegions * (totalRegions - 1) / 2;
        i128 pairsDiff = totalPairs - withinPairs;

        // sum_dH weighted over center pairs
        i128 hexUp = hexSumWithin(up);
        i128 hexDown = hexSumWithin(down);

        i128 sum_dH = 2*hexUp + 2*hexDown;

        i128 Wup = 0, Wdown = 0;
        for (auto &c : up) Wup += c.w;
        for (auto &c : down) Wdown += c.w;

        i128 hexCross = hexSumBetween(up, down);

        // count_s = sum_{up} w_u * sum_{down: s_d < s_u} w_d
        vector<pair<int,int64>> downS;
        downS.reserve(down.size());
        for (auto &c : down) downS.push_back({c.s, c.w});
        sort(downS.begin(), downS.end(), [](auto &A, auto &B){ return A.first < B.first; });
        vector<int> ds;
        ds.reserve(downS.size());
        vector<i128> prefDownW(downS.size()+1,0);
        for (size_t i=0;i<downS.size();i++){
            ds.push_back(downS[i].first);
            prefDownW[i+1] = prefDownW[i] + (i128)downS[i].second;
        }
        i128 count_s = 0;
        for (auto &c : up) {
            int s = c.s;
            size_t pos = lower_bound(ds.begin(), ds.end(), s) - ds.begin(); // s_d < s
            count_s += (i128)c.w * prefDownW[pos];
        }

        sum_dH += Wup*Wdown + 2*hexCross - 2*count_s;

        // directed usable starts (ordered count)
        vector<pair<int,int64>> allX, allY, upSList, downSList;
        allX.reserve(up.size()+down.size());
        allY.reserve(up.size()+down.size());
        upSList.reserve(up.size());
        downSList.reserve(down.size());
        for (auto &c : up) {
            allX.push_back({c.x, c.w});
            allY.push_back({c.y, c.w});
            upSList.push_back({c.s, c.w});
        }
        for (auto &c : down) {
            allX.push_back({c.x, c.w});
            allY.push_back({c.y, c.w});
            downSList.push_back({c.s, c.w});
        }
        sort(allX.begin(), allX.end(), [](auto &A, auto &B){ return A.first < B.first; });
        sort(allY.begin(), allY.end(), [](auto &A, auto &B){ return A.first < B.first; });
        sort(upSList.begin(), upSList.end(), [](auto &A, auto &B){ return A.first < B.first; });
        sort(downSList.begin(), downSList.end(), [](auto &A, auto &B){ return A.first < B.first; });

        vector<int> xs, ys, usVals, dsVals;
        xs.reserve(allX.size()); ys.reserve(allY.size());
        usVals.reserve(upSList.size()); dsVals.reserve(downSList.size());
        vector<i128> prefX(allX.size()+1,0), prefY(allY.size()+1,0), prefUS(upSList.size()+1,0), prefDS(downSList.size()+1,0);
        for (size_t i=0;i<allX.size();i++){ xs.push_back(allX[i].first); prefX[i+1]=prefX[i]+(i128)allX[i].second; }
        for (size_t i=0;i<allY.size();i++){ ys.push_back(allY[i].first); prefY[i+1]=prefY[i]+(i128)allY[i].second; }
        for (size_t i=0;i<upSList.size();i++){ usVals.push_back(upSList[i].first); prefUS[i+1]=prefUS[i]+(i128)upSList[i].second; }
        for (size_t i=0;i<downSList.size();i++){ dsVals.push_back(downSList[i].first); prefDS[i+1]=prefDS[i]+(i128)downSList[i].second; }

        i128 totalW = prefX.back();
        i128 totalU = prefUS.back();
        i128 totalD = prefDS.back();

        auto countXlt = [&](int x)->i128 {
            size_t pos = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
            return prefX[pos];
        };
        auto countXgt = [&](int x)->i128 {
            size_t pos = upper_bound(xs.begin(), xs.end(), x) - xs.begin();
            return totalW - prefX[pos];
        };
        auto countYlt = [&](int y)->i128 {
            size_t pos = lower_bound(ys.begin(), ys.end(), y) - ys.begin();
            return prefY[pos];
        };
        auto countYgt = [&](int y)->i128 {
            size_t pos = upper_bound(ys.begin(), ys.end(), y) - ys.begin();
            return totalW - prefY[pos];
        };

        i128 ordered = 0;
        // up sources
        for (auto &c : up) {
            // side 1: Y' < Y
            ordered += (i128)c.a[1] * countYlt(c.y);
            // side 2: X' < X
            ordered += (i128)c.a[2] * countXlt(c.x);
            // side 0: target up S'>S ; target down S'>=S
            int s = c.s;
            size_t posU = upper_bound(usVals.begin(), usVals.end(), s) - usVals.begin(); // <=s
            i128 cntU = totalU - prefUS[posU]; // >s
            size_t posD = lower_bound(dsVals.begin(), dsVals.end(), s) - dsVals.begin(); // <s
            i128 cntD = totalD - prefDS[posD]; // >=s
            ordered += (i128)c.a[0] * (cntU + cntD);
        }
        // down sources
        for (auto &c : down) {
            // side 1: X' > X
            ordered += (i128)c.a[1] * countXgt(c.x);
            // side 2: Y' > Y
            ordered += (i128)c.a[2] * countYgt(c.y);
            // side 0: target up S'<=S ; target down S'<S
            int s = c.s;
            size_t posU = upper_bound(usVals.begin(), usVals.end(), s) - usVals.begin(); // <=s
            i128 cntU = prefUS[posU];
            size_t posD = lower_bound(dsVals.begin(), dsVals.end(), s) - dsVals.begin(); // <s
            i128 cntD = prefDS[posD];
            ordered += (i128)c.a[0] * (cntU + cntD);
        }

        // incompatibilities
        i128 incompat = 0;

        // (0,0): same S but not same (X,Y)
        unordered_map<int, int64> upA0ByS, downA0ByS;
        upA0ByS.reserve(up.size()*2);
        downA0ByS.reserve(down.size()*2);
        unordered_map<uint64_t, int64> upA0ByXY, downA0ByXY;
        upA0ByXY.reserve(up.size()*2);
        downA0ByXY.reserve(down.size()*2);

        for (auto &c : up) {
            upA0ByS[c.s] += c.a[0];
            upA0ByXY[packXY(c.x,c.y)] += c.a[0];
        }
        for (auto &c : down) {
            downA0ByS[c.s] += c.a[0];
            downA0ByXY[packXY(c.x,c.y)] += c.a[0];
        }

        i128 inc00 = 0;
        for (auto &kv : upA0ByS) {
            auto it = downA0ByS.find(kv.first);
            if (it != downA0ByS.end()) {
                inc00 += (i128)kv.second * (i128)it->second;
            }
        }
        i128 sub00 = 0;
        for (auto &kv : upA0ByXY) {
            auto it = downA0ByXY.find(kv.first);
            if (it != downA0ByXY.end()) {
                sub00 += (i128)kv.second * (i128)it->second; // adjacent equal coords
            }
        }
        incompat += inc00 - sub00;

        // (1,2): Y_u = Y_d + 1 and X_u != X_d
        unordered_map<int, int64> upA1ByY, downA2ByY;
        upA1ByY.reserve(up.size()*2);
        downA2ByY.reserve(down.size()*2);
        unordered_map<uint64_t, int64> upA1ByXY, downA2ByXY;
        upA1ByXY.reserve(up.size()*2);
        downA2ByXY.reserve(down.size()*2);

        for (auto &c : up) {
            upA1ByY[c.y] += c.a[1];
            upA1ByXY[packXY(c.x,c.y)] += c.a[1];
        }
        for (auto &c : down) {
            downA2ByY[c.y] += c.a[2];
            downA2ByXY[packXY(c.x,c.y)] += c.a[2];
        }

        i128 inc12 = 0;
        for (auto &kv : upA1ByY) {
            int y = kv.first;
            auto it = downA2ByY.find(y-1);
            if (it != downA2ByY.end()) {
                inc12 += (i128)kv.second * (i128)it->second;
            }
        }
        i128 sub12 = 0; // subtract X equal adjacency: (x,y) with (x,y-1)
        for (auto &kv : upA1ByXY) {
            // up at (x,y), need down at (x, y-1)
            uint64_t key = kv.first;
            int x = (int)((uint32_t)(key >> 32) ^ 0x80000000u);
            int y = (int)((uint32_t)(key & 0xffffffffu) ^ 0x80000000u);
            uint64_t downKey = packXY(x, y-1);
            auto it = downA2ByXY.find(downKey);
            if (it != downA2ByXY.end()) sub12 += (i128)kv.second * (i128)it->second;
        }
        incompat += inc12 - sub12;

        // (2,1): X_u = X_d + 1 and Y_u != Y_d
        unordered_map<int, int64> upA2ByX, downA1ByX;
        upA2ByX.reserve(up.size()*2);
        downA1ByX.reserve(down.size()*2);
        unordered_map<uint64_t, int64> upA2ByXY, downA1ByXY;
        upA2ByXY.reserve(up.size()*2);
        downA1ByXY.reserve(down.size()*2);

        for (auto &c : up) {
            upA2ByX[c.x] += c.a[2];
            upA2ByXY[packXY(c.x,c.y)] += c.a[2];
        }
        for (auto &c : down) {
            downA1ByX[c.x] += c.a[1];
            downA1ByXY[packXY(c.x,c.y)] += c.a[1];
        }

        i128 inc21 = 0;
        for (auto &kv : upA2ByX) {
            int x = kv.first;
            auto it = downA1ByX.find(x-1);
            if (it != downA1ByX.end()) {
                inc21 += (i128)kv.second * (i128)it->second;
            }
        }
        i128 sub21 = 0; // subtract Y equal adjacency: (x,y) with (x-1,y)
        for (auto &kv : upA2ByXY) {
            uint64_t key = kv.first;
            int x = (int)((uint32_t)(key >> 32) ^ 0x80000000u);
            int y = (int)((uint32_t)(key & 0xffffffffu) ^ 0x80000000u);
            uint64_t downKey = packXY(x-1, y);
            auto it = downA1ByXY.find(downKey);
            if (it != downA1ByXY.end()) sub21 += (i128)kv.second * (i128)it->second;
        }
        incompat += inc21 - sub21;

        // Final answer
        i128 ans = sameCenterSum + (2*sum_dH + pairsDiff - ordered + incompat);
        cout << toString128(ans) << "\n";
    }
    return 0;
}