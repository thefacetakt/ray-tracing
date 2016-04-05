#ifndef RT_TRIANGLE
#define RT_TRIANGLE

#include "Figure.hpp"
#include "BasicGeom.hpp"
#include "Float.hpp"

using namespace Float;
using namespace BasicGeom;

class Triangle: public Figure {
    static const int SIDES = 3;
    Vector V[SIDES];

    bool inside(const Vector &v) const {
        for (int i = 0; i < SIDES; ++i) {
            if (!collinearIfParralel(
                    (V[(i + 1) % SIDES] - V[i]) % (v - V[i]),
                    (V[(i + 1) % SIDES] - V[i])
                    % (V[(i + 2) % SIDES] - V[i]))) {
                return false;
            }
        }
        return true;
    }

public:
    Triangle(const Vector &a, const Vector &b, const Vector &c) {
        V[0] = a, V[1] = b, V[2] = c;
    }

    Vector rayIntersection(const Ray &r) const {
        Vector dir1 = V[1] - V[0];
        Vector dir2 = V[2] - V[0];
        Plane self = Plane(V[0], dir1, dir2);
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
