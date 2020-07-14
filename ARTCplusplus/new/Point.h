//
// Created by Muhammad Ashfaq on 7/6/2020.
//

#ifndef ARTCPLUSPLUS_POINT_H
#define ARTCPLUSPLUS_POINT_H

#include <iostream>
#include <vector>
#include <random>

using namespace std;

class Point {
public:
    int n;  // Dimensions
    vector<float> coordPoint;   // Store the coordinate values of each dimension of the point
    float* coordPointNew;   // Store the coordinate values of each dimension of the point
    Point() {}    // Constructor1
    Point(int n) {     // Constructor2
        this->n = n;
        coordPointNew = new float[n];
    }

    static double getDistance(Point p1, Point p2) { // Get a Euclidean distance between two test points
        double sum = 0;
        for (int i = 0; i < p1.n; i++) {
            sum = sum + pow(p1.coordPoint[i] - p2.coordPoint[i], 2);
        }
        return sqrt(sum);
    }

    static double getDistance(Point p1, Point* p2) { // Get a Euclidean distance between two test points
        double sum = 0;
        for (int i = 0; i < p1.n; i++) {
            sum = sum + pow(p1.coordPoint[i] - p2->coordPoint[i], 2);
        }
        return sqrt(sum);
    }

    void printPoint() {
        cout << '(';
        for (int i=0; i < this->coordPoint.size(); i++)
            if (i != (this->coordPoint.size() - 1))
                cout << this->coordPoint[i] << ',';
            else
                cout << this->coordPoint[i];
        cout << "),";
    }

    static Point generateRandP(vector<vector<float> > inputDomain) {
        //for random numbers, todo: move these 3 lines out somewhere
        std::random_device device;
        std::mt19937 generator(device());
        // std::mt19937 generator(seed); for setting seed
        std::uniform_real_distribution<float> myrand(0, 1);

        int n = inputDomain.size();
        Point newPoint = Point(n);
        for (int i = 0; i < n; i++) {
            newPoint.coordPoint.push_back(
                    (float) (inputDomain[i][0] + (inputDomain[i][1] - inputDomain[i][0]) * myrand(generator)));
        }
        return newPoint;
    }

    static Point newGenerateRandP(int inputDomain[2][2]) {
        //for random numbers, todo: move these 3 lines out somewhere
        std::random_device device;
        std::mt19937 generator(device());
        // std::mt19937 generator(seed); for setting seed
        std::uniform_real_distribution<float> myrand(0, 1);
        int n = sizeof(inputDomain[0])/sizeof(int);
        Point newPoint = Point(n);
        for (int i = 0; i < n; i++) {
            newPoint.coordPointNew[i] = 
                    (float) (inputDomain[i][0] + (inputDomain[i][1] - inputDomain[i][0]) * myrand(generator));
        }
        return newPoint;
    }

};

#endif

//ARTCPLUSPLUS_POINT_H
