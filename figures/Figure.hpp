#ifndef RT_FIGURE
#define RT_FIGURE

#include "BoundingBox.hpp"

struct Figure {
    virtual Vector rayIntersection(const Ray &r) const = 0;
    virtual Plane getTangentPlane(const Vector &point) const = 0;
    virtual BoundingBox getBoundingBox() const = 0;
    virtual myFloat getBoundingBox(int dim, int side) const {
        return getBoundingBox()[dim][side];
    }

    virtual ~Figure() {
    }
};

#endif
