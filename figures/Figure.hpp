#ifndef RT_FIGURE
#define RT_FIGURE

#include "BoundingBox.hpp"
#include <string>

using std::string;

struct Figure {
    virtual Vector rayIntersection(const Ray &r) const = 0;
    virtual Plane getTangentPlane(const Vector &point) const = 0;
    virtual BoundingBox getBoundingBox() const = 0;
    virtual myFloat getBoundingBox(int dim, int side) const {
        return getBoundingBox()[dim][side];
    }
    virtual string name() const = 0;
    virtual bool on(const Vector &) const = 0;
    virtual ~Figure() {
    }
};

#endif
