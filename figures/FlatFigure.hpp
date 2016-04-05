#ifndef RT_FLAT_FIGURE
#define RT_FLAT_FIGURE

#include "Figure.hpp"
#include "../geometry/BasicGeom.hpp"
#include <cstdio>


using namespace BasicGeom;

class FlatFigure : public Figure{
    virtual Plane getContainingPlane() const = 0;
    virtual bool inside(const Vector &v) const = 0;
public:

    virtual ~FlatFigure() {

    }

    Plane getTangentPlane(const Vector &point) const {
        return getContainingPlane();
    }

    Vector rayIntersection(const Ray &r) const {
        Plane self = getContainingPlane();
        Vector candidate = intersect(r, self);
        if (inside(candidate)) {
                return candidate;
        }
        return NONE;
    }
};

#endif
