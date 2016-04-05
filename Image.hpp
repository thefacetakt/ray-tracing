#ifndef RT_IMAGE
#define RT_IMAGE

#include <vector>
#include <cstddef>
#include <png++/png.hpp>

using std::vector;

class Image {
public:
    struct RGB {
        unsigned char R, G, B;
        unsigned char& operator[](size_t i) {
            if (i == 0) {
                return R;
            }
            if (i == 1) {
                return G;
            }
            if (i == 2) {
                return B;
            }
            throw "RNG index out of range";
        }

        RGB() {
            R = G = B = 0;
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

#endif
