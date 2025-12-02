#include "testlib.h"
#include <cmath>
 
using namespace std;
 
const double EPS = 1E-6;
 
int main(int argc, char * argv[])
{
    setName("compare two sequences of doubles, max absolute or relative  error = %.7f", EPS);
    registerTestlibCmd(argc, argv);
    int n = inf.readInt(1, 200000, "n");
    for (int i = 0; i < n; i++) {
        string expected = ans.readLine();
        string result = ouf.readLine();
        if (expected == "-1") {
            if (result != "-1") {
                quitf(_wa, "expected -1, found %s", result.c_str());
            }
        } else {
            double expected_d = atof(expected.c_str());
            double result_d = atof(result.c_str());
            if (!doubleCompare(expected_d, result_d, EPS)) {
                quitf(_wa, "expected %s, found %s", expected.c_str(), result.c_str());
            }
        }
    }
    quitf(_ok, "all %d numbers are correct", n);
}
