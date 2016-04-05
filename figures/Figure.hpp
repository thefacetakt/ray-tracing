#ifndef RT_FIGURE
#define RT_FIGURE

#include "../geometry/BasicGeom.hpp"

using namespace BasicGeom;

struct Figure {
    virtual Vector rayIntersection(const Ray &r) const = 0;
    virtual Plane getTangentPlane(const Vector &point) const = 0;
    virtual ~Figure() {
    }
};

#endif
