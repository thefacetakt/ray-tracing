#ifndef RT_FLAT_FIGURE
#define RT_FLAT_FIGURE

#include "Figure.hpp"
#include "BasicGeom.hpp"

using namespace BasicGeom;

class FlatFigure : Figure{
    virtual Plane getContainingPlane() = 0;
    virtual bool inside(const Vector &v) = 0;
public:

    virtual ~FlatFigure() {

    }

    Vector rayIntersection(const Ray &r) const {
        Vector dir1 = V[1] - V[0];
        Vector dir2 = V[2] - V[0];
        Plane self = getContainingPlane();
        Vector selfPoint = self.getPoint();
        Plane p1 = Plane(r.start, r.direction, dir1);
        Plane p2 = Plane(r.start, r.direction, dir2);
        Vector candidate = ThreePlanesIntersection(self, p1, p2);
        //if candidate is NONE everything is OK too
        if (inside(candidate)) {
            //TODO why less??
            if (lessOrEqual(r.direction * (candidate - r.start), 0.)) {
                return candidate;
            }
        }
        return NONE;
    }
};

#endif
