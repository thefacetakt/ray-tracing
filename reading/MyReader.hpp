#ifndef RT_MY_READER
#define RT_MY_READER

#include "../figures/Triangle.hpp"
#include "../figures/Quadrangle.hpp"
#include "../figures/Figure.hpp"
#include "../figures/Sphere.hpp"
#include "../rendering/Image.hpp"
#include "../scene/OneColorBody.hpp"
#include "../scene/TexturedBody.hpp"
#include "../figures/VolumetricFigure.hpp"
#include <cstdio>
#include <cstring>
#include <string>

using std::string;

/*
* T - number of triangles
* T lines: x1 y1 z1 x2 y2 z2 x3 y3 z3 R G B mirroring
* S - number of spheres
* S lines: x0 y0 z0 r R G B mirroring
*/
const char *NO_TEXTURE = "none";
const int MAX_TEXTURE_NAME_LENGTH = 100;

OneColorBody::Properties scanfProperties(FILE *in) {
    Image::RGB color;
    color.scanfColor(in);
    color.setRandomIfNull();
    myFloat reflection, refraction;
    assert(fscanf(in, "%lf %lf", &reflection, &refraction) == 2);
    return {color, reflection, refraction};
}

vector <IBody *> readMy(const char *filename) {
    vector <IBody *> result;
    if (filename == NULL) {
        return result;
    }
    FILE *in = fopen(filename, "r");
    int T, S, Q;
    assert(fscanf(in, "%d", &T) == 1);
    for (int z = 0; z < T; ++z) {
        Triangle* current = new Triangle();
        for (int i = 0; i < Triangle::size(); ++i) {
            (*current)[i].scanfVector(in);
        }
        char textureName[MAX_TEXTURE_NAME_LENGTH];
        auto properties = scanfProperties(in);

        assert(fscanf(in, "%s", textureName) == 1);

        if (strcmp(textureName, NO_TEXTURE)) {
            string name(textureName);
            TexturedBody::addTexture(name);
            result.push_back(new TexturedBody(current, name,
                                              properties.reflection,
                                              properties.refraction));
        } else {
            result.push_back(new OneColorBody(properties, current));
        }
    }

    assert(fscanf(in, "%d", &S) == 1);
    for (int z = 0; z < S; ++z) {
        Sphere *current = new Sphere();
        current->O.scanfVector(in);
        assert(fscanf(in, "%lf", &current->R) == 1);
        result.push_back(new OneColorBody(scanfProperties(in), current));
    }

    assert(fscanf(in, "%d", &Q) == 1);
    for (int z = 0; z < Q; ++z) {
        Quadrangle* current = new Quadrangle();
        for (int i = 0; i < Quadrangle::size(); ++i) {
            (*current)[i].scanfVector(in);
        }
        char textureName[MAX_TEXTURE_NAME_LENGTH];
        auto properties = scanfProperties(in);

        assert(fscanf(in, "%s", textureName) == 1);

        if (strcmp(textureName, NO_TEXTURE)) {
            string name(textureName);
            TexturedBody::addTexture(name);
            result.push_back(new TexturedBody(current, name,
                                              properties.reflection,
                                              properties.refraction));
        } else {
            result.push_back(new OneColorBody(properties, current));
        }
    }
    int SS;
    assert(fscanf(in, "%d", &SS) == 1);
    for (int i = 0; i < SS; ++i) {
        Sphere *current = new Sphere();
        current->O.scanfVector(in);
        assert(fscanf(in, "%lf", &current->R) == 1);
        Sphere *current1 = new Sphere();
        current1->O.scanfVector(in);
        assert(fscanf(in, "%lf", &current1->R) == 1);
        result.push_back(new OneColorBody(scanfProperties(in),
                         new IntersectionFigure(
                             (VolumetricFigure *)(current1),
                             (VolumetricFigure *)(current))));
    }
    fclose(in);
    return result;
}

#endif
