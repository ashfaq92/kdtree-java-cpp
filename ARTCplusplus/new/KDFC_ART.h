//
// Created by Muhammad Ashfaq on 7/7/2020.
//

#ifndef ARTCPLUSPLUS_KDFC_ART_H
#define ARTCPLUSPLUS_KDFC_ART_H

#include <stack>
#include <queue>
#include <cfloat>
#include <vector>
#include "Node.h"


using namespace std;

class KDFC_ART {
public:
    Node* root;
    int size = 0;
    int candidateNum = 10;
    int* newInputDomain;
    vector<vector<float>> inputDomain;
    int rows;
    int columns;

    KDFC_ART(int* bound, int rows, int columns) {
        this->root = new Node();
        this->newInputDomain = bound;
        this->rows = rows;
        this->columns = columns;
    }

    stack<Node*> getTreePath(Point p) {
        stack<Node*> path;
        Node* pathNode = this->root;

        while (true) {
            path.push(pathNode);
            if (pathNode->p->coordPoint[pathNode->split] > p.coordPoint[pathNode->split]) {
                if (pathNode->left == nullptr) {
                    break;
                }
                pathNode = pathNode->left;
            } else {
                if (pathNode->right == nullptr) {
                    break;
                }
                pathNode = pathNode->right;
            }
        }
        return path;
    }

    int judgeDirection(Point p, Node* node) {
        if (p.coordPoint[node->split] < node->p->coordPoint[node->split]) {
            return 0;
        } else {
            return 1;
        }
    }

    bool isCrossSplitLine(Point p, double distance, Node* node) {
        if (abs(node->p->coordPoint[node->split] - p.coordPoint[node->split]) >= distance) {
            return false;
        }
        return true;
    }

    double getMinDisByBacktracking(Point p, int back) {
        int num  = 0;
        stack<Node*> path = this->getTreePath(p);
        Node* pathNode = new Node();
        double distance = DBL_MAX;

        while (!path.empty()) {
            pathNode = path.top();
            path.pop();

            if (this->isCrossSplitLine(p, distance, pathNode)) {
                double d = Point::getDistance(p, pathNode->p);
                if (distance > d) {
                    distance = d;
                }
                num++;
                if (num == back) {
                    return distance;
                }
                int direction = this->judgeDirection(p, pathNode);
                Node* tempNode = new Node();
                if (direction == 0) {
                    if (pathNode->right != nullptr) {
                        tempNode = pathNode->right;
                    }
                } else {
                    if (pathNode->left != nullptr) {
                        tempNode = pathNode->left;
                    }
                }

                if (tempNode != nullptr) {
                    queue<Node*> queue;
                    queue.push(tempNode);
                    while (!queue.empty()) {
                        tempNode = queue.front();
                        queue.pop();
                        direction = this->judgeDirection(p, tempNode);

                        if(this->isCrossSplitLine(p, distance, tempNode)) {
                            d = Point::getDistance(p, tempNode->p);
                            if (distance > d) {
                                distance = d;
                            }
                            num++;
                            if (num == back) {
                                return distance;
                            }
                            if (direction == 1) {
                                if (tempNode->left != nullptr) {
                                    queue.push(tempNode->left);
                                }
                            } else {
                                if (tempNode->right != nullptr) {
                                    queue.push(tempNode->right);
                                }
                            }
                        } else {
                            num++;
                            if (num == back) {
                                return distance;
                            }
                        }
                        if (direction == 0) {
                            if (tempNode->left != nullptr) {
                                queue.push(tempNode->left);
                            }
                        } else {
                            if (tempNode->right != nullptr) {
                                queue.push(tempNode->right);
                            }
                        }
                    }
                }
            } else {
                num++;
                if (num == back) {
                    return distance;
                }
            }
        }
        return distance;
    }

    int splitSelect(vector<vector<float> > boundary, Point* p) {
        double rate = 0;
        int split = 0;
        for (int i = 0; i<p->n; i++) {
            double length = boundary[i][1] - boundary[i][0];
            double lx1 = boundary[i][1] - p->coordPoint[i];
            double lx2 = p->coordPoint[i] - boundary[i][0];
            double spread = length * (1 - (lx1 / length) * (lx1 / length) - (lx2 / length) * (lx2 / length));
            if (rate < spread) {
                rate = spread;
                split = i;
            }
        }
        return split;
    }

    void insertPointByStrategy(Point* p) {    
        if (root->p == nullptr) {
            root->deep = 1;
            root->p = p;
            root->newBoundary = new int*[p->n];
            for (int i = 0; i < 2; i++) 
                root->newBoundary[i] = new int[2];
            for (int i = 0; i < p->n; i++){
                root->newBoundary[i][0] = this->inputDomain[i][0];
                root->newBoundary[i][1] = this->inputDomain[i][1];
            }
            root->split = this->splitSelect(root->boundary, p);
        } else {
            Node* ntemp = root;
            Node* n = new Node();
            //while (true) {
                
                if(ntemp->p->coordPointNew[ntemp->split] > p->coordPointNew[ntemp->split]) {                
                    if (ntemp->left == nullptr) {
                        ntemp->left = n;
               //         break;
                    }
                    ntemp = ntemp->left;
                } 
                return;/*
                else {
                    if (ntemp->right == nullptr) {
                        ntemp->right = n;
              //          break;
                    }
                    ntemp = ntemp->right;
                }*/
            //}
            n->p = p;        
            n->newBoundary = ntemp->newBoundary;
            n->deep = ntemp->deep+1;
            if (n->p->coordPoint[ntemp->split] < ntemp->p->coordPoint[ntemp->split]) {
                n->newBoundary[ntemp->split][1] = ntemp->p->coordPoint[ntemp->split];
               //*( ((n->newBoundary) + ntemp->split) + 1) = ntemp->p->coordPoint[ntemp->split];
            } else {
                n->newBoundary[ntemp->split][0] = ntemp->p->coordPoint[ntemp->split];
            }
            //n->split = this->splitSelect(n->newboundary, p);
        }
        size++;
    }

	int buildIndex(vector<Point> & dataSet, bool debug) {  //returns KDTree of  data-set
		double d = this->columns;  // dimensionality of input domain
		int back = 0;
        int dataSetSize = dataSet.size();
		int* backNum = new int[dataSetSize];    
		backNum[1] = 1;
		for (int i = 2; i < dataSetSize; i++) {
            backNum[i] = (int) ceil(1 / 2.0 * pow((d + 1 / d), 2) * (log(i) / log(2)));
		}
		for (int i = 0; i < dataSetSize; i ++){
            Point p = dataSet.at(i);
			back = backNum[this->size];
			this->insertPointByStrategy(&p); //FIXME
			if (debug){p.printPoint();}   // if debug is true, print the dataset point to visualize them on www.desmos.com/calculator
		}
		return back;
	}

    void testLimBalKDFC_Efficiency(int pointNum, vector<int> backNum, bool debug=false) {
        Point p = Point::generateRandP(this->inputDomain);
        this->insertPointByStrategy(&p);
        Point finalCase;
        vector<Point> canD;
        for (int i = 0; i < pointNum; i++) {
            //initialize new arraylist, not needed
            for (int j = 0; j < this->candidateNum; j++) {
                finalCase = canD.at(0);
                int back = backNum[this->size];
                double distance = this->getMinDisByBacktracking(finalCase, back);
                for (int c = 0; c < candidateNum; c++) {
                    double d = this->getMinDisByBacktracking(canD.at(c), back);
                    if (distance < d) {
                        distance = d;
                        finalCase = canD.at(c);
                    }
                }
                this->insertPointByStrategy(&finalCase);
                if (debug) {
                    finalCase.printPoint();
                }
            }
        }
    }
};

#endif //ARTCPLUSPLUS_KDFC_ART_H
