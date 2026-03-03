#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);
    quitf(_ok, "interactive: verdict decided by interactor");
}
