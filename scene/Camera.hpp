#ifndef RT_CAMERA
#define RT_CAMERA

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"
#include "../containers/Container.hpp"
#include "Scene.hpp"
#include <algorithm>


using std::max;
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
        this->stepX = stepX.normed();
        stepY = this->stepX % (direction.normed());
    }

    Vector getPixel(float x, float y) const {
        return position + direction + stepX * x + stepY * y;
    }
};

Image view(const Camera &camera, const Scene *scene,
    int height, int width) {
    Image result(width, height);
    pair<int, int> leftmost(-height / 2, -width / 2);
    for (int y = -height / 2; y < height / 2; ++y) {
        for (int x = -width / 2; x < width / 2; ++x) {
            result(height / 2 + y, width / 2 + x)
                = scene->color(Ray(camera.getPosition(),
                                   camera.getPixel(x, y)));
            auto tmp = result(height / 2 + y, width / 2 + x);
            if (tmp.R != 0 || tmp.G != 0 || tmp.B != 0) {
                if (x > leftmost.second) {
                    leftmost = make_pair(y, x);
                }
            }
        }
    }
    return result;
}



#endif
