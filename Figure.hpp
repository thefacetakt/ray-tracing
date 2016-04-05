#ifndef RT_FIGURE
#define RT_FIGURE

#include "BasicGeom.hpp"

using namespace BasicGeom;

struct Figure {
    virtual Vector rayIntersection(const Ray &r) const = 0;

    virtual ~Figure() {
    }
};

#endif
