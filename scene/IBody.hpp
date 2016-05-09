#ifndef RT_IBODY
#define RT_IBODY

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"

struct IBody {
    virtual const Figure * getFigure() const = 0;
    virtual Image::RGB getColor(const Vector &point) const = 0;
    virtual myFloat getReflection() const = 0;
    virtual ~IBody() {
    }
};

#endif
