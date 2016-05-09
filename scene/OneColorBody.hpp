#ifndef RT_ONE_COLOR_BODY
#define RT_ONE_COLOR_BODY

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"
#include "IBody.hpp"

struct OneColorBody : public IBody{
    struct Properties {
        Image::RGB color;
        myFloat alpha;
    };

    Properties properties;
    Figure *figure;

    OneColorBody() {
        figure = NULL;
        properties.alpha = 0;
    }

    OneColorBody(const Properties &properties, Figure * const & figure)
        : properties(properties), figure(figure) {
    }

    OneColorBody(Figure * const & figure)
        : figure(figure) {
        properties.color = GREY;
        properties.alpha = 1.0;
    }

    const Figure * getFigure() const {
        return figure;
    }

    Image::RGB getColor(const Vector &) const {
        return properties.color;
    }

    myFloat getReflection() const {
        return properties.alpha;
    }

    virtual ~OneColorBody() {
        delete figure;
    }
};

#endif
