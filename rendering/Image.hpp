#ifndef RT_IMAGE
#define RT_IMAGE

#include <vector>
#include <cstddef>
#include <png++/png.hpp>
#include <string>
#include "../geometry/Float.hpp"

using namespace Float;
using std::vector;
using std::string;

class Image {
public:
    struct RGB {
        unsigned char R, G, B;
        unsigned char& operator[](size_t i) {
            switch (i) {
                case 0:
                    return R;
                break; case 1:
                    return G;
                break; case 2:
                    return B;
            }
        }

        RGB() {
            R = G = B = 0;
        }

        void scanfColor(FILE *in) {
            assert(fscanf(in, "%hhu%hhu%hhu", &R, &G, &B) == 3);
        }

        void setRandomIfNull() {
            if (R == 0 && G == 0 && B == 0) {
                R = rand() % 255;
                G = rand() % 255;
                B = rand() % 255;
            }
        }
        unsigned char colorFromFloat(float x) const {
            if (less(x, 0.)) {
                return 0;
            }
            if (greater(x, 255)) {
                return 255;
            }
            return (unsigned char)(x);
        }

        RGB operator*(myFloat increase) const {
            return RGB(colorFromFloat(R * increase),
                       colorFromFloat(G * increase),
                       colorFromFloat(B * increase));
        }

        RGB operator+(const RGB &other) const {
            return RGB(colorFromFloat(R + other.R),
                       colorFromFloat(G + other.G),
                       colorFromFloat(B + other.B));
        }

        RGB(png::rgb_pixel pixel) {
            R = pixel.red;
            G = pixel.green;
            B = pixel.blue;
        }

        RGB(unsigned char r, unsigned char g, unsigned char b)
            : R(r), G(g), B(b) {
        }
    };
private:
    size_t width;
    size_t height;
    vector <vector <RGB> > body;

public:


    Image(size_t width, size_t height) {
        this->width = width;
        this->height = height;
        body.assign(height, vector<RGB>(width));
    }

    Image(string name) {
        png::image< png::rgb_pixel > image(name);
        height = image.get_height();
        width = image.get_width();
        body.assign(height, vector<RGB>(width));
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                body[y][x] = RGB(image[y][x]);
            }
        }
    }

    Image() {
        width = height = 0;
    }

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
        return height;
    }

    RGB& operator()(size_t i, size_t j) {
        return body[i][j];
    }

    void write(const char *filename) {
        png::image< png::rgb_pixel > image(width, height);
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                image[y][x] = png::rgb_pixel(body[y][x].R,
                    body[y][x].G, body[y][x].B);
            }
        }
        image.write(filename);
    }
};

static const Image::RGB GREY = Image::RGB(100, 100, 100);

#endif
