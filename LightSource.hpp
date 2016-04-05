#ifndef RT_LIGHT_SOURCE
#define RT_LIGHT_SOURCE

#include "geometry/BasicGeom.hpp"

using namespace BasicGeom;

struct LightSource {
    Vector position;
    float energy;

    float getIncrease(const Vector &point, const Vector &normal) const {
        return energy / pow((position - point).len(), 3.)
               * (normal * (position - point));
    }
};

#endif
