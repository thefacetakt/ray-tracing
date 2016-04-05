#ifndef RT_CAMERA
#define RT_CAMERA

#include "Image.hpp"
#include "Figure.hpp"
#include <vector>

using std::vector;

class Camera {
    Vector position;
    Vector direction;
    Vector stepX;
    Vector stepY;
public:
    const Vector& getPosition() const{
        return position;
    }

    Camera(const Vector &position, const Vector &direction,
           const Vector &stepX) {
        this->position = position;
        this->direction = direction;
        this->stepX = stepX;
        stepY = stepX % (direction.normed());
    }

    Vector getPixel(float x, float y) const {
        return direction + stepX * x + stepY * y;
    }
};

Image view(const Camera &camera, const vector<Figure *> &figures,
    int height, int width) {
    //TODO: changeit
    Image::RGB color(255, 0, 0);
    Image result(width, height);

    for (int y = -height / 2; y < height / 2; ++y) {
        for (int x = -width / 2; x < width / 2; ++x) {
            result(height / 2 + y, width / 2 + x) = Image::RGB(0, 0, 0);
            for (const auto &figure: figures) {
                if (!(figure->rayIntersection(Ray(camera.getPosition(),
                                            camera.getPixel(x, y))) == NONE)) {
                    result(height / 2 + y, width / 2 + x) = color;
                }
            }
        }
    }
    return result;
}



#endif
