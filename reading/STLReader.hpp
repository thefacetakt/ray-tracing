#ifndef RT_STL_READER
#define RT_STL_READER

#include "../figures/Triangle.hpp"
#include "../figures/Figure.hpp"
#include "../figures/Sphere.hpp"
#include <cstdio>
#include <cstring>

const char *VERTEX = "vertex";
const char *SPHERE = "sphere";
const char *BAD_FORMAT = "bad stl file";
vector <Figure *> readSTL(const char *filename) {
    vector <Figure *> result;
    if (filename == NULL) {
        return result;
    }
    FILE *in = fopen(filename, "r");
    static const int MAX_LINE_LENGTH = 80;
    char s[MAX_LINE_LENGTH];
    while (fscanf(in, "%s", s) == 1) {
        if (strcmp(s, VERTEX) == 0) {
            Triangle* current = new Triangle();
            for (int i = 0; i < Triangle::size(); ++i) {
                if (fscanf(in, "%lf %lf %lf", &(*current)[i].x,
                           &(*current)[i].y,
                           &(*current)[i].z) != Triangle::size()) {
                    throw BAD_FORMAT;
                }
                // (*current)[i].x *= 100;
                // (*current)[i].y *= 100;
                // (*current)[i].z *= 100;
                if (i + 1 != Triangle::size()) {
                    if (fscanf(in, "%s", s) != 1) { //here should be vertex word
                        throw BAD_FORMAT;
                    }
                }
            }
            result.push_back(dynamic_cast<Figure *>(current));
        } else if (strcmp(s, SPHERE) == 0) {
            Sphere *sphere = new Sphere();
            if (!sphere->fscanfSelf(in)) {
                throw BAD_FORMAT;
            }
            result.push_back(dynamic_cast<Figure *>(sphere));
        }
    }
    fclose(in);
    return result;
}

#endif
