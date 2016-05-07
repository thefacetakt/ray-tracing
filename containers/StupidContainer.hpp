#ifndef RT_STUPID_CONTAINER
#define RT_STUPID_CONTAINER

#include "../scene/Body.hpp"
#include "../rendering/Image.hpp"
#include "../reading/STLReader.hpp"
#include "Container.hpp"
#include "../geometry/BasicGeom.hpp"
#include "../figures/Sphere.hpp"
#include <vector>
#include <utility>

using std::pair;
using std::make_pair;
using std::vector;

using namespace BasicGeom;

class StupidContainer: public Container {
    vector<Body> bodies;
public:
    StupidContainer(const char *filename) {
        vector <Body *> bodiesPtr = readSTL(filename);
        bodies.resize(bodiesPtr.size());
        for (size_t i = 0; i < bodiesPtr.size(); ++i) {
            bodies[i].properties = bodiesPtr[i]->properties;
            bodies[i].figure = bodiesPtr[i]->figure;
            bodiesPtr[i]->figure = NULL;
            delete bodiesPtr[i];
        }
    }

    pair<Vector, const Body *> rayIntersection(const Ray &ray) const {
        double minimalTime = 1e18;
        const Body *currentBody = NULL;

        for (auto const &body: bodies) {
            Vector candidate = body.figure->rayIntersection(ray);
            if (candidate != NONE) {
                double currentTime = (candidate - ray.start) * ray.direction;

                if (less(currentTime, minimalTime)) { //and time != 0, perhaps??
                    minimalTime = currentTime;
                    //TODO: it will not always be so;
                    currentBody = &body;
                }
            }
        }
        if (!eq(minimalTime, 1e18)) {
            return make_pair(ray.start +  ray.direction * minimalTime,
                             currentBody);
        }
        return make_pair(NONE, currentBody);
    }

    virtual ~StupidContainer() {
    }
};

#endif
