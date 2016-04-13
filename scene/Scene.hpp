#ifndef RT_SCENE
#define RT_SCENE

#include "LightSource.hpp"
#include "../geometry/BasicGeom.hpp"
#include "../containers/Container.hpp"
#include "../containers/StupidContainer.hpp"
#include "../containers/KDTree.hpp"
#include "../reading/LightsLoader.hpp"
#include <vector>

using std::vector;

class Scene {
    vector <LightSource> lights;
    Container *container;
public:

    Scene(const char *lightsFile, const char *figuresFile) {
        lights = readLightSources(lightsFile);
        container = new KDTree(figuresFile);
    }

    Image::RGB color(const Ray &cameraRay) const {
        auto camViewPoint = container->rayIntersection(cameraRay);
        if (camViewPoint.first != NONE) {
            float increase = 0;
            for (auto &lamp: lights) {
                auto lampPoint
                    = container->rayIntersection(Ray(lamp.position,
                                                     camViewPoint.first,
                                                     START_POINT));
                if (lampPoint == camViewPoint) {
                    Vector normal = camViewPoint.second->figure
                                        ->getTangentPlane(camViewPoint.first).n;
                    if (greaterOrEqual(normal * cameraRay.direction, 0.)) {
                        normal = -normal;
                    }
                    float currentIncrease = fabs(lamp.getIncrease(camViewPoint.first,
                                                             normal));
                    if (greaterOrEqual(currentIncrease, 0.)) {
                        increase += currentIncrease;
                    } else {

                    }
                }
            }
            return camViewPoint.second->properties.color * increase;
        }
        return Image::RGB(0, 0, 0);
    }
    ~Scene() {
        delete container;
    }
};

#endif
