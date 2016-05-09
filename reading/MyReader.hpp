#ifndef RT_MY_READER
#define RT_MY_READER

#include "../figures/Triangle.hpp"
#include "../figures/Figure.hpp"
#include "../figures/Sphere.hpp"
#include "../rendering/Image.hpp"
#include "../scene/OneColorBody.hpp"
#include "../scene/TexturedBody.hpp"
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
    myFloat alpha;
    assert(fscanf(in, "%Lf", &alpha) == 1);
    return {color, alpha};
}

vector <IBody *> readMy(const char *filename) {
    vector <IBody *> result;
    if (filename == NULL) {
        return result;
    }
    FILE *in = fopen(filename, "r");
    int T, S;
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
            result.push_back(new TexturedBody(current, name, properties.alpha));
        } else {
            result.push_back(new OneColorBody(properties, current));
        }
    }

    assert(fscanf(in, "%d", &S) == 1);
    for (int z = 0; z < S; ++z) {
        Sphere *current = new Sphere();
        current->O.scanfVector(in);
        assert(fscanf(in, "%Lf", &current->R) == 1);
        result.push_back(new OneColorBody(scanfProperties(in), current));
    }
    fclose(in);
    return result;
}

#endif
