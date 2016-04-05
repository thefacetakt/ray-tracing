#ifndef RT_STUPID_CONTAINER
#define RT_STUPID_CONTAINER

#include "../Body.hpp"
#include "../Image.hpp"
#include "../STLReader.hpp"
#include "Container.hpp"
#include "../geometry/BasicGeom.hpp"
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
        vector <Figure *> figures = readSTL(filename);
        bodies.resize(figures.size());
        for (size_t i = 0; i < figures.size(); ++i) {
            bodies[i].figure = figures[i];
            bodies[i].properties.color = Image::RGB(rand() % 255, rand() % 255,
                                                    rand() % 255);
        }
    }

    pair<Vector, Image::RGB> rayIntersection(const Ray &ray) const {
        double minimalTime = 1e18;
        Image::RGB currentColor;

        for (auto const &body: bodies) {
            Vector candidate = body.figure->rayIntersection(ray);
            if (candidate != NONE) {
                double currentTime = (candidate - ray.start) * ray.direction;

                if (less(currentTime, minimalTime)) { //and time != 0, perhaps??
                    minimalTime = currentTime;
                    //TODO: it will not always be so;
                    currentColor = body.properties.color;
                }
            }
        }
        if (!eq(minimalTime, 1e18)) {
            return make_pair(ray.start +  ray.direction * minimalTime,
                             currentColor);
        }
        return make_pair(NONE, Image::RGB(0, 0, 0));
    }

    virtual ~StupidContainer() {
    }
};

#endif
