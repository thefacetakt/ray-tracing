#ifndef RT_QUADRANGLE
#define RT_QUADRANGLE

#include "Figure.hpp"
#include "FlatFigure.hpp"
#include "../geometry/BasicGeom.hpp"
#include "Triangle.hpp"
#include <algorithm>

using std::rotate;
using std::min;
using std::max;

using namespace Float;
using namespace BasicGeom;

class Quadrangle: public FlatFigure {
    static const int SIDES = 4;
    Vector V[SIDES];
    bool inside(const Vector &v) const {
        return (Triangle(V[0], V[1], V[2]).inside(v)
                || Triangle(V[0], V[3], V[2]).inside(v));
    }

    Plane getContainingPlane() const {
        Vector dir1 = V[1] - V[0];
        Vector dir2 = V[2] - V[0];
        return Plane(V[0], dir1, dir2);
    }
public:
    Quadrangle() {
    }

    Quadrangle(const Vector &a, const Vector &b, const Vector &c,
             const Vector &d) {
        V[0] = a, V[1] = b, V[2] = c, V[3] = d;
        if (collinearIfParralel((b - a) % (c - a), (d - a) % (c - a))) {
            std::rotate(V, V + 1, V + SIDES);
        }
    }

    static size_t size() {
        return SIDES;
    }

    Vector &operator[](size_t i) {
        if (i >= SIDES) {
            throw "Quadrangle only has four sides";
        }
        return V[i];
    }

    BoundingBox getBoundingBox() const {
        BoundingBox result;
        for (int i = 0; i < DIMENSIONS; ++i) {
            result[i][0] = min(min(V[0][i], V[1][i]), min(V[2][i], V[3][i]));
            result[i][1] = max(max(V[0][i], V[1][i]), max(V[2][i], V[3][i]));
        }
        return result;
    }

    myFloat getBoundingBox(int dim, int side) const {
        if (side == 0) {
            return min(V[0][dim], min(V[1][dim], V[2][dim]));
        } else {
            return max(V[0][dim], max(V[1][dim], V[2][dim]));
        }
    }

    Vector getPoint() const {
        return V[0];
    }

    Vector getOX() const {
        return (V[1] - V[0]).normed();
    }

    string name() const {
        return "Quadrangle";
    }
};

#endif
