//
// Created by Muhammad Ashfaq on 7/6/2020.
//

#ifndef ARTCPLUSPLUS_NODE_H
#define ARTCPLUSPLUS_NODE_H

#include "Point.h"

class Node {
public:
    int split;
    Node *left;
    Node *right;
    Node *parent;
    Point *p;
    vector<vector<float> > boundary;
    int** newBoundary;

    int deep;

    // Node() {
    //     this->left = nullptr;
    //     this->right = nullptr;
    //     this->parent = nullptr;
    //     this->p = NULL;
    //     this->deep = NULL;
    //     this->split = NULL;
    // }
    Node() :
            left(nullptr),
            right(nullptr),
            deep(0),
            p(nullptr) {

    }
};


#endif //ARTCPLUSPLUS_NODE_H
