#ifndef RT_MY_READER
#define RT_MY_READER

#include "../figures/Triangle.hpp"
#include "../figures/Figure.hpp"
#include "../figures/Sphere.hpp"
#include "../rendering/Image.hpp"
#include "../scene/Body.hpp"
#include <cstdio>
#include <cstring>

/*
* T - number of triangles
* T lines: x1 y1 z1 x2 y2 z2 x3 y3 z3 R G B mirroring
* S - number of spheres
* S lines: x0 y0 z0 r R G B mirroring
*/

Body::Properties scanfProperties(FILE *in) {
    Image::RGB color;
    color.scanfColor(in);
    color.setRandomIfNull();
    myFloat alpha;
    assert(fscanf(in, "%lf", &alpha) == 1);
    return {color, alpha};
}

vector <Body *> readMy(const char *filename) {
    vector <Body *> result;
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
        result.push_back(new Body(scanfProperties(in), current));
    }

    assert(fscanf(in, "%d", &S) == 1);
    for (int z = 0; z < S; ++z) {
        Sphere *current = new Sphere();
        current->O.scanfVector(in);
        assert(fscanf(in, "%lf", &current->R) == 1);
        result.push_back(new Body(scanfProperties(in), current));
    }
    fclose(in);
    return result;
}

#endif
