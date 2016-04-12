#ifndef RT_KD_TREE
#define RT_KD_TREE

#include "Container.hpp"
#include "../figures/Figure.hpp"
#include <vector>
#include <algorithm>

using std::vector;
using std::nth_element;


class KDTree: public Container {
    struct Node {
        BoundingBox bounds;
        Node *left, *right;
        Figure *figure;
        Node() {
            figure = NULL;
            left = right = NULL;
        }
    };

    Node *makeTree(vector<Figure *>::iterator begin,
                   vector<Figure *>::iterator end, int step) {
        if (begin == end) {
            return NULL;
        }
        int n = end - begin;
        
    }
public:
    virtual pair<Vector, const Body *> rayIntersection(const Ray &) const = 0;
};

#endif
