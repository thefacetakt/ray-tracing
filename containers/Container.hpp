#ifndef RT_CONTAINER
#define RT_CONTAINER

#include "../geometry/BasicGeom.hpp"
#include "../scene/IBody.hpp"
#include "../rendering/Image.hpp"
#include <utility>

using std::pair;

using namespace BasicGeom;

class Container {
public:
    virtual pair<Vector, const IBody *> rayIntersection(const Ray &) const = 0;
    virtual ~Container() {
    };
};

#endif
