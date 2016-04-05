#ifndef RT_SPHERE
#define RT_SPHERE

#include "Figure.hpp"
#include "../geometry/BasicGeom.hpp"

using namespace Float;
using namespace BasicGeom;

class Sphere: public Figure {
    Vector O;
    float R;

public:
    Sphere() {
    }

    Sphere(const Vector &O, const float &R) : O(O), R(R) {
    }
};

#endif
