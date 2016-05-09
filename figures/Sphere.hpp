#ifndef RT_SPHERE
#define RT_SPHERE

#include "Figure.hpp"
#include "../geometry/BasicGeom.hpp"
#include <algorithm>
#include <cstdio>

using std::min;

using namespace Float;
using namespace BasicGeom;

struct Sphere: public Figure {
    Vector O;
    myFloat R;

public:
    Sphere() {
    }

    Sphere(const Vector &O, const myFloat &R) : O(O), R(R) {
    }

    Plane getTangentPlane(const Vector &point) const {
        return Plane(point, point - O);
    }

    Vector rayIntersection(const Ray &r) const {
        Vector normalUp = ((O - r.start) % r.direction);
        myFloat distance2 = normalUp.len2() / r.direction.len2();
        if (greater(distance2, sq(R))) {
            return NONE;
        }
        myFloat startDistance = sqrt((O - r.start).len2() - distance2);
        Vector normedDirection = r.direction.normed();
        Vector projection = r.start + normedDirection * startDistance;
        myFloat intersecDistance = sqrt(sq(R) - distance2);

        Vector candidate1 = projection - normedDirection * intersecDistance;
        Vector candidate2 = projection + normedDirection * intersecDistance;

        myFloat t1 = (candidate1 - r.start) * normedDirection;
        myFloat t2 = (candidate2 - r.start) * normedDirection;
        t1 = (lessOrEqual(t1, 0.) ? 1e18 : t1);
        t2 = (lessOrEqual(t2, 0.) ? 1e18 : t2);
        myFloat t = min(t1, t2);
        if (eq(t, 1e18)) {
            return NONE;
        }
        return r.start + normedDirection * t;
    }

    bool fscanfSelf(FILE *in) {
        if (fscanf(in, "%Lf %Lf %Lf %Lf", &O.x, &O.y, &O.z, &R) != 4) {
            return false;
        }
        return true;
    }

    BoundingBox getBoundingBox() const {
        BoundingBox result;
        for (int i = 0; i < DIMENSIONS; ++i) {
            result[i][0] = O[i] - R;
            result[i][1] = O[i] + R;
        }
        return result;
    }

    myFloat getBoundingBox(int dim, int side) const {
        return O[dim] + (2 * side - 1) * R;
    }

    string name() const {
        return "Sphere";
    }

    bool on(const Vector &point) const {
        return eq((point - O).len2(), sq(R));
    }
};

#endif
