#ifndef RT_SCENE
#define RT_SCENE

#include "LightSource.hpp"
#include "../geometry/BasicGeom.hpp"
#include "../containers/Container.hpp"
#include "../containers/StupidContainer.hpp"
#include "../containers/KDTree.hpp"
#include "../reading/LightsLoader.hpp"
#include <vector>
#include <iostream>


using std::vector;
using std::cout;

class Scene {
    static const int MAX_REFLECTION_DEPTH = 1;
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

    Image::RGB color(const Ray &cameraRay, int reflectionDepth=0,
                     myFloat currentRefraction=1.) const {
        auto camViewPoint = container->rayIntersection(cameraRay);
        if (camViewPoint.first != NONE) {
            if (!eq(currentRefraction, 1.)
                || !eq(camViewPoint.second->getRefraction(), 1.)) {
                myFloat nextRefraction = (eq(currentRefraction, 1.)
                                             ? camViewPoint.second
                                               ->getRefraction()
                                             : 1.);

                 auto refracted
                    = refraction(
                        cameraRay.start,
                        camViewPoint.first,
                        camViewPoint.second->getFigure()
                            ->getTangentPlane(camViewPoint.first).n,
                        nextRefraction / currentRefraction
                    );
                  if (refracted != NONE) {
                      return color(Ray(
                          camViewPoint.first,
                          refracted),
                          reflectionDepth,
                          nextRefraction
                       );
                   }
            }

            myFloat increase = 0.2;
            for (auto &lamp: lights) {
                auto lampPoint
                    = container->rayIntersection(Ray(lamp.position,
                                                     camViewPoint.first,
                                                     START_POINT), 0);
                if (lampPoint == camViewPoint) {
                    Vector normal = camViewPoint.second->getFigure()
                                        ->getTangentPlane(camViewPoint.first).n;
                    if (greaterOrEqual(normal * cameraRay.direction, 0.)) {
                        normal = -normal;
                    }
                    float currentIncrease = fabs(lamp.getIncrease(
                                                             camViewPoint.first,
                                                             normal));
                    if (greaterOrEqual(currentIncrease, 0.)) {
                        increase += currentIncrease;
                    }
                }
            }

            Image::RGB myColor
                = camViewPoint.second->getColor(camViewPoint.first) * increase;

            Image::RGB reflectedColor;
            if (reflectionDepth != MAX_REFLECTION_DEPTH
                && !eq(camViewPoint.second->getReflection(), 0.)) {
                reflectedColor
                    = color(Ray(
                                camViewPoint.first,
                                reflection(
                                    cameraRay.start,
                                    camViewPoint.first,
                                    camViewPoint.second->getFigure()
                                        ->getTangentPlane(camViewPoint.first).n
                                ),
                                START_POINT
                            ),
                            reflectionDepth + 1,
                            1.0
                      );
            }

            return myColor * (1 - camViewPoint.second->getReflection())
                  + reflectedColor * camViewPoint.second
                                              ->getReflection();
        }
        return Image::RGB(0, 0, 0);
    }
    ~Scene() {
        delete container;
    }
};

#endif
