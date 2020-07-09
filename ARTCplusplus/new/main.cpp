#include <iostream>
#include <vector>
#include <cmath> //for log function
#include "Point.h"
#include "Node.h"
//#include "fscs/art/FSCS_ART.h"
#include "KDFC_ART.h"

using namespace std;

void limBalKdfcWrapper() {
    // required parameters
    //vector<vector<float> > bd = {{0.0, 1.0}, {0.0,1,}};
	int bd[2][2] = {{-5000, 5000}, {-5000, 5000}};
    
    int bdSize = sizeof(bd[0])/sizeof(int);
    int pointNum = 500;

    // preliminary steps
    double d = bdSize;
    vector<Point> backNum = vector<Point> (pointNum);
    backNum[1] = 1;
    for (int i = 2; i < pointNum; i++) {
        Point p = Point::newGenerateRandP(bd);
        backNum[i] = (int) ceil(1 / 2.0 * pow((d + 1 / d), 2) * (log(i) / log(2)));
    }        
  //  KDFC_ART kdfc = KDFC_ART(bd);
  //  kdfc.testLimBalKDFC_Efficiency(pointNum, backNum, true);
}

int main() {
    cout << "Hello, World!" << endl;
    int bd[2][2] = {{-5000, 5000}, {-5000, 5000}};
    int bdSize = sizeof(bd[0])/sizeof(int);
    int rows = bdSize, columns = bdSize;
    int pointNum = 500;

    double d = bdSize;
    vector<Point> randData = vector<Point>(pointNum);
    for (int i = 0; i < pointNum; i++) {
        Point p = Point::newGenerateRandP(bd);
        randData.push_back(p);
    }

    KDFC_ART kdfc = KDFC_ART(bd[0], rows, columns);
    kdfc.buildIndex(randData, true);
    
    return 0;

}