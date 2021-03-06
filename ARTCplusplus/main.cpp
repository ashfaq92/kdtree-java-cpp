#include <iostream>
#include <vector>
#include <cmath> //for log function
#include "auxiliary/model/Point.h"
#include "auxiliary/model/Node.h"
#include "fscs/art/FSCS_ART.h"
#include "kdfc/art/KDFC_ART.h"

using namespace std;

void limBalKdfcWrapper() {
    // required parameters
    vector<vector<float> > bd = {{0.0, 1.0}, {0,1}};
    int pointNum = 500;

    // preliminary steps
    double d = bd.size();
    vector<int> backNum = vector<int> (pointNum);
    backNum[1] = 1;
    for (int i = 2; i < pointNum; i++) {
        backNum[i] = (int) ceil(1 / 2.0 * pow((d + 1 / d), 2) * (log(i) / log(2)));
    }
    KDFC_ART kdfc = KDFC_ART(bd);
    kdfc.testLimBalKDFC_Efficiency(pointNum, backNum, true);
}

int main() {
    cout << "Hello, World!" << endl;
    // Node a;
    // vector<float> cordp;
    // cordp.push_back(2.0f);
    // // cout<<cordp[0];
    //
    // vector<vector<float> > bd = {{0.0, 1.0}, {0,1}};
    // Point b = Point::generateRandP(bd);
    // b.printPoint();

    // FSCS_ART myfscs = FSCS_ART(10) ;
    // myfscs.testFscsArt_Efficiency(500, bd, true);

    limBalKdfcWrapper();
    return 0;

}