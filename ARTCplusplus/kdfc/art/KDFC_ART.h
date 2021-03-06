//
// Created by Muhammad Ashfaq on 7/7/2020.
//

#ifndef ARTCPLUSPLUS_KDFC_ART_H
#define ARTCPLUSPLUS_KDFC_ART_H

#include <stack>
#include <queue>
#include <cfloat>
#include <vector>
#include "../../auxiliary/model/Node.h"


using namespace std;

class KDFC_ART {
public:
    Node* root;
    int size = 0;
    int candidateNum = 10;
    vector<vector<float>> inputDomain;

    KDFC_ART(vector<vector<float>> bound) {
        this->root = new Node();
        this->inputDomain = bound;
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
            root->p = p; //todo: is this okay or do I pass parameter "Point* p" ??
            // root.boundary = vector ( p.n , vector<float > (2, 0));
            root->boundary = this->inputDomain;
            root->split = this->splitSelect(root->boundary, p);
        } else {
            Node* ntemp = root;
            Node* n = new Node();
            while (true) {
                if(ntemp->p->coordPoint[ntemp->split] > p->coordPoint[ntemp->split]) {
                    if (ntemp->left == nullptr) {
                        ntemp->left = n;
                        break;
                    }
                    ntemp = ntemp->left;
                } else {
                    if (ntemp->right == nullptr) {
                        ntemp->right = n;
                        break;
                    }
                    ntemp = ntemp->right;
                }
            }
            n->p = p;
            n->boundary = ntemp->boundary;
            n->deep = ntemp->deep+1;
            ntemp->split;
            if (n->p->coordPoint[ntemp->split] < ntemp->p->coordPoint[ntemp->split]) {
                n->boundary[ntemp->split][1] = ntemp->p->coordPoint[ntemp->split];
            } else {
                n->boundary[ntemp->split][0] = ntemp->p->coordPoint[ntemp->split];
            }
            n->split = this->splitSelect(n->boundary, p);
        }
        size++;
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
