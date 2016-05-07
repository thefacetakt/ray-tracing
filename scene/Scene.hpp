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
    static const int MAX_DEPTH = 2;
    vector <LightSource> lights;
    Container *container;
public:

    Scene(const char *lightsFile, const char *figuresFile) {
        lights = readLightSources(lightsFile);
        EFileMode mode;
        int lengthOfFigiresFileName =strlen(figuresFile);
        if (figuresFile[lengthOfFigiresFileName - 1] == 'l') { //'stl'
            mode = STL;
        } else {
            mode = MY_OWN;
        }
        container = new KDTree(figuresFile, mode);
    }

    Image::RGB color(const Ray &cameraRay, int depth=0) const {
        auto camViewPoint = container->rayIntersection(cameraRay);
        if (camViewPoint.first != NONE) {
            float increase = 0.1;
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
                    float currentIncrease = fabs(lamp.getIncrease(
                                                             camViewPoint.first,
                                                             normal));
                    if (greaterOrEqual(currentIncrease, 0.)) {
                        increase += currentIncrease;
                    } else {

                    }
                }
            }

            Image::RGB myColor
                = camViewPoint.second->properties.color * increase;
            Image::RGB reflectedColor;
            if (depth != MAX_DEPTH) {
                reflectedColor
                    = color(Ray(camViewPoint.first,
                                reflection(
                                    cameraRay.start,
                                    camViewPoint.first,
                                    camViewPoint.second->figure
                                        ->getTangentPlane(camViewPoint.first).n
                                ),
                                START_POINT
                            ),
                            depth + 1
                      );
            }
            auto ret = myColor * camViewPoint.second->properties.alpha
                  + reflectedColor * (1 - camViewPoint.second
                                              ->properties.alpha);
            return ret;
        }
        return Image::RGB(0, 0, 0);
    }
    ~Scene() {
        delete container;
    }
};

#endif
