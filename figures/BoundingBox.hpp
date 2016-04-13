#ifndef RT_BOUNDING_BOX
#define RT_BOUNDING_BOX

#include "../geometry/BasicGeom.hpp"
#include <algorithm>

using std::min;
using std::max;

using namespace BasicGeom;

struct BoundingBox {
    myFloat boundaries[DIMENSIONS][2];

    myFloat* operator[](size_t i) {
        return boundaries[i];
    }

    const myFloat* operator[](size_t i) const {
        return boundaries[i];
    }

    bool intersects(const Ray &ray) const {

        for (int i = 0; i < DIMENSIONS; ++i) {
            for (int j = 0; j < 2; ++j) {
                myFloat t;
                if (eq(ray.direction[i], 0.)) {
                    if (eq(ray.start[i], boundaries[i][j])) {
                        t = 0;
                    }
                    else {
                        continue;
                    }
                } else {
                    t = (boundaries[i][j] - ray.start[i]) / ray.direction[i];
                }
                if (less(t, 0.)) {
                    continue;
                }
                Vector candidate = ray.start + ray.direction * t;
                bool fail = false;
                for (int k = 1; k < DIMENSIONS; ++k) {
                    if (!(less(boundaries[(i + k) % DIMENSIONS][0],
                               candidate[(i + k) % DIMENSIONS]) &&
                          less(candidate[(i + k) % DIMENSIONS],
                               boundaries[(i + k) % DIMENSIONS][1]))) {
                        fail = true;
                    }
                }
                if (!fail) {
                    return true;
                }
            }
        }
        return false;
    }
};

BoundingBox operator|(const BoundingBox &a, const BoundingBox &b) {
    BoundingBox result;
    for (int i = 0; i < DIMENSIONS; ++i) {
        result[i][0] = min(a[i][0], b[i][0]);
        result[i][1] = max(a[i][1], b[i][1]);
    }
    return result;
}

bool intersects(const myFloat *first, const myFloat *second) {
    return greaterOrEqual(min(first[1], second[1]), max(first[0], second[0]));
}

bool intersects(const BoundingBox &a, const BoundingBox &b) {
    for (int i = 0; i < DIMENSIONS; ++i) {
        if (!intersects(a[i], b[i])) {
            return false;
        }
    }
    return true;
}

//is first inside second
bool isInside(const myFloat *first, const myFloat *second) {
    return lessOrEqual(second[0], first[0]) && lessOrEqual(first[1], second[1]);
}

bool isInside(const BoundingBox &a, const BoundingBox &b) {
    for (int i = 0; i < DIMENSIONS; ++i) {
        if (!isInside(a, b)) {
            return false;
        }
    }
    return true;
}


#endif
