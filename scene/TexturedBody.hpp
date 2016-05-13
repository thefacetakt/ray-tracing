#ifndef RT_TEXTURED_BODY
#define RT_TEXTURED_BODY

#include "IBody.hpp"
#include "../rendering/Image.hpp"
#include "../figures/FlatFigure.hpp"

#include <map>
#include <string>

using std::map;
using std::string;

long long mod(long long x, long long y) {
    if (x < 0) {
        x += (100 - x / y) * y;
    }
    return x % y;
}

struct TexturedBody : public IBody {
private:
    myFloat reflection;
    myFloat refraction;
    string textureName;
    FlatFigure *figure;
public:
    static map<string, Image> texturesPool;

    const Figure * getFigure() const {
        return figure;
    }

    myFloat getReflection() const {
        return reflection;
    }

    myFloat getRefraction() const {
        return refraction;
    }

    Image::RGB getColor(const Vector &v) const {
        static const long long NORMING = 1;
        Vector coords = (v - figure->getPoint());
        Image &texture = texturesPool[textureName];
        long long x = (coords * figure->getOX()) * NORMING,
                  y = (coords * figure->getOY()) * NORMING;
        x = mod(x, texture.getWidth());
        y = mod(y, texture.getHeight());
        return texture(y, x);
    }

    TexturedBody(FlatFigure * const &figure, string textureName,
                 myFloat reflection=0, myFloat refraction=0) {
        this->figure = figure;
        this->textureName = textureName;
        this->reflection = reflection;
        this->refraction = refraction;
    }

    virtual ~TexturedBody() {
        delete figure;
    }

    static void addTexture(string name) {
        if (texturesPool.find(name) == texturesPool.end()) {
            texturesPool[name] = Image(name);
        }
    }
};

map<string, Image> TexturedBody::texturesPool = map<string, Image>();



#endif
