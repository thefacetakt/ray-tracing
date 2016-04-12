#ifndef RT_FIGURE
#define RT_FIGURE

#include "../geometry/BasicGeom.hpp"

using namespace BasicGeom;

struct BoundingBox {
    myFloat boundaries[DIMENSIONS][2];

    myFloat* operator[](size_t i) {
        return boundaries[i];
    }
};

struct Figure {
    virtual Vector rayIntersection(const Ray &r) const = 0;
    virtual Plane getTangentPlane(const Vector &point) const = 0;
    virtual BoundingBox getBoundingBox() const = 0;

    virtual ~Figure() {
    }
};

#endif
