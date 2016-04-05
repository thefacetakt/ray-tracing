#include "Image.hpp"
#include "figures/Triangle.hpp"
#include "Camera.hpp"

#ifdef TEST_PNG

#include <png++/png.hpp>
void testPng() {
    png::image< png::rgb_pixel > image(128, 128);
    for (png::uint_32 y = 0; y < image.get_height(); ++y)
    {
        for (png::uint_32 x = 0; x < image.get_width(); ++x)
        {
            image[y][x] = png::rgb_pixel(x, y, x + y + 100);
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    image.write("rgb.png");
}
#endif

int main() {
    Camera camera(Vector(0, 0, 0),
                  Vector(0, 0, 1),
                  Vector(1, 0, 0));
    Figure* figure = dynamic_cast<Figure*>(
        new Triangle(Vector(-100, -100, 2),
                     Vector(-100, 100, 1),
                     Vector(100, 100, 1)));
    Image image = view(camera, {figure}, 1080, 1920);
    delete figure;
    image.write("triangle.png");

    return 0;
}
