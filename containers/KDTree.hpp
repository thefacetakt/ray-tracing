#ifndef RT_KD_TREE
#define RT_KD_TREE

#include "Container.hpp"
#include "../figures/Figure.hpp"
#include "../reading/STLReader.hpp"
#include <vector>
#include <algorithm>

using std::vector;
using std::nth_element;

int steps = 0;

class KDTree: public Container {
    struct Node {
        BoundingBox bounds;
        Node *left, *right;
        Body *body;
        Node() {
            body = NULL;
            left = right = NULL;
        }
        Node (Body *body) : body(body) {
            bounds = body->figure->getBoundingBox();
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

    Node *makeTree(vector<Body *>::iterator begin,
                   vector<Body *>::iterator end, int step) {
        if (begin == end) {
            return NULL;
        }
        int n = end - begin;
        nth_element(begin, begin + n / 2, end,
                    [&step] (const Body * a, const Body *b) {
                        return a->figure->getBoundingBox(step, 0) <
                        b->figure->getBoundingBox(step, 0);
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
                   const Body * &currentIntersection,
                   const Ray &currentRay) const {
        ++steps;
        if (!v || !(v->bounds.intersects(currentRay))) {
            return;
        }
        Vector current = v->body->figure->rayIntersection(currentRay);
        if (current != NONE) {
            myFloat myTime = (current - currentRay.start)
                              * currentRay.direction;
            if (greaterOrEqual(myTime, 0.) && less(myTime, currentTime)) {
                currentTime = myTime;
                currentIntersection = v->body;
            }
        }
        intersect(v->left, currentTime, currentIntersection, currentRay);
        intersect(v->right, currentTime, currentIntersection, currentRay);
    }

public:
    KDTree(const char *filename) {
        vector <Figure *> figures = readSTL(filename);
        vector <Body *> bodies(figures.size());
        for (size_t i = 0; i < figures.size(); ++i) {
            bodies[i] = new Body({Image::RGB(100, 100, 100)}, //rand() % 255, rand() % 255,
                                                    //rand() % 255)},
                                 figures[i]);
        }
        root = makeTree(bodies.begin(), bodies.end(), 0);
        for (int i = 0; i < 3; ++i) {
            printf("%.3f %.3f\n", root->bounds[i][0], root->bounds[i][1]);
        }
        printf("%d\n", bodies.size());
    }
    virtual pair<Vector, const Body *> rayIntersection(const Ray &ray) const {
        myFloat currentTime = 1e18;
        const Body * currentIntersection = NULL;
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
