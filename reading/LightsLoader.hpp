#ifndef RT_LIGHT_LOADER
#define RT_LIGHT_LOADER

#include "../scene/LightSource.hpp"
#include <vector>
#include <cstdio>

using std::vector;

const char *BAD_FILE = "Bad lights sources file";

vector<LightSource> readLightSources(const char *filename) {
    vector <LightSource> result;
    if (filename == NULL) {
        return result;
    }
    FILE *in = fopen(filename, "r");
    int n;
    if (fscanf(in, "%d", &n) != 1) {
        throw BAD_FILE;
    }
    for (int i = 0; i < n; ++i) {
        LightSource source;
        if (fscanf(in, "%lf%lf%lf%lf", &source.position.x, &source.position.y,
                               &source.position.z, &source.energy) != 4) {
            throw BAD_FILE;
        }
        result.push_back(source);
    }
    fclose(in);
    return result;
}

#endif
