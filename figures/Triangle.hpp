#ifndef RT_TRIANGLE
#define RT_TRIANGLE

#include "Figure.hpp"
#include "FlatFigure.hpp"
#include "../geometry/BasicGeom.hpp"

using namespace Float;
using namespace BasicGeom;

class Triangle: public FlatFigure {
    static const int SIDES = 3;
    Vector V[SIDES];
    bool inside(const Vector &v) const {
        for (int i = 0; i < SIDES; ++i) {
            if (!collinearIfParralel(
                    (V[(i + 1) % SIDES] - V[i]) % (v - V[i]),
                    (V[(i + 1) % SIDES] - V[i])
                    % (V[(i + 2) % SIDES] - V[i]))) {
                auto x1 = (V[(i + 1) % SIDES] - V[i]) % (v - V[i]);
                auto x2 = (V[(i + 1) % SIDES] - V[i]) % (V[(i + 2) % SIDES] - V[i]);
                int z = 1 + 2;
                return false;
            }
        }
        return true;
    }

    Plane getContainingPlane() const {
        Vector dir1 = V[1] - V[0];
        Vector dir2 = V[2] - V[0];
        return Plane(V[0], dir1, dir2);
    }
public:
    Triangle() {
    }

    Triangle(const Vector &a, const Vector &b, const Vector &c) {
        V[0] = a, V[1] = b, V[2] = c;
    }

    static size_t size() {
        return SIDES;
    }

    Vector &operator[](size_t i) {
        if (i >= SIDES) {
            throw "triangle only has three sides";
        }
        return V[i];
    }
};

#endif
