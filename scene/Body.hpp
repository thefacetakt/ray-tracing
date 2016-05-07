#ifndef RT_BODY
#define RT_BODY

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"

struct Body {
    struct Properties {
        Image::RGB color;
        myFloat alpha;
    };

    Properties properties;
    Figure *figure;

    Body() {
        figure = NULL;
        properties.alpha = 0;
    }

    Body(const Properties &properties, Figure * const & figure)
        : properties(properties), figure(figure) {
    }

    ~Body() {
        delete figure;
    }
};

#endif
