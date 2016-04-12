#ifndef RT_BODY
#define RT_BODY

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"

struct Body {
    struct Properties {
        Image::RGB color;
    };

    Properties properties;
    Figure *figure;

    Body() {
        figure = NULL;
    }

    Body(const Properties &properties, Figure *& figure)
        : properties(properties), figure(figure) {
    }

    ~Body() {
        delete figure;
    }
};

#endif
