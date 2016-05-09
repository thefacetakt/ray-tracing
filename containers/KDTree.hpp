#ifndef RT_KD_TREE
#define RT_KD_TREE

#include "Container.hpp"
#include "../figures/Figure.hpp"
#include "../reading/STLReader.hpp"
#include "../reading/MyReader.hpp"
#include <vector>
#include <algorithm>

using std::vector;
using std::nth_element;

int steps = 0;

enum EFileMode {
    STL,
    MY_OWN,
};

class KDTree: public Container {
    struct Node {
        BoundingBox bounds;
        Node *left, *right;
        IBody *body;
        Node() {
            body = NULL;
            left = right = NULL;
        }
        Node (IBody *body) : body(body) {
            bounds = body->getFigure()->getBoundingBox();
            left = right = NULL;
        }
        ~Node() {
            delete body;
            delete left;
            delete right;
        }
    };

    Node *root;

    void recalc(Node *v) {
        if (v->left) {
            v->bounds = (v->bounds | v->left->bounds);
        }
        if (v->right) {
            v->bounds = (v->bounds | v->right->bounds);
        }
    }

    Node *makeTree(vector<IBody *>::iterator begin,
                   vector<IBody *>::iterator end, int step) {
        if (begin == end) {
            return NULL;
        }
        int n = end - begin;
        nth_element(begin, begin + n / 2, end,
                    [&step] (const IBody * a, const IBody *b) {
                        return a->getFigure()->getBoundingBox(step, 0) <
                        b->getFigure()->getBoundingBox(step, 0);
                    });
        Node *result = new Node(*(begin + n / 2));
        result->left = makeTree(begin, begin + (n / 2),
                                (step + 1) % DIMENSIONS);
        result->right = makeTree(begin + (n / 2) + 1, end,
                                (step + 1) % DIMENSIONS);
        recalc(result);
        return result;
    }

    void intersect(Node *v, myFloat &currentTime,
                   const IBody * &currentIntersection,
                   const Ray &currentRay) const {
        ++steps;
        if (!v || !(v->bounds.intersects(currentRay))) {
            return;
        }
        if (!v->body->getFigure()->on(currentRay.start)) {
            Vector current = v->body->getFigure()->rayIntersection(currentRay);
            if (current != NONE) {
                myFloat myTime = (current - currentRay.start)
                                  * currentRay.direction;
                if (greater(myTime, 0.) && less(myTime, currentTime)) {
                    currentTime = myTime;
                    currentIntersection = v->body;
                }
            }
        }
        intersect(v->left, currentTime, currentIntersection, currentRay);
        intersect(v->right, currentTime, currentIntersection, currentRay);
    }

public:
    KDTree(const char *filename, EFileMode mode) {
        vector <IBody *> bodies;
        switch (mode) {
            case STL:
                bodies = readSTL(filename);
            break; case MY_OWN:
                bodies = readMy(filename);
            break;
        }
        root = makeTree(bodies.begin(), bodies.end(), 0);

#ifdef RT_DEBUG
        fprintf(stderr, "Bounding box:\n");
        for (int i = 0; i < 3; ++i) {
            fprintf(stderr, "%.3Lf %.3Lf\n", root->bounds[i][0],
                    root->bounds[i][1]);
        }
        fprintf(stderr, "Figures: %zu\n", bodies.size());
#endif
    }

    virtual pair<Vector, const IBody *> rayIntersection(const Ray &ray) const {
        myFloat currentTime = 1e18;
        const IBody * currentIntersection = NULL;
        intersect(root, currentTime, currentIntersection, ray);
        if (eq(currentTime, 1e18)) {
            return {NONE, NULL};
        }
        return {ray.start + ray.direction * currentTime, currentIntersection};
    }

    ~KDTree() {
        delete root;
    }
};

#endif
