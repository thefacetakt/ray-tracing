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
    int width;
    int height;
public:
    const Vector& getPosition() const{
        return position;
    }

    Camera(const Vector &position, const Vector &direction,
           const Vector &stepX, int width, int height) {
        this->position = position;
        this->direction = direction;
        this->stepX = stepX.normed();
        stepY = this->stepX % (direction.normed());
    }

    Camera(const char *filename) {
        FILE *in = fopen(filename, "r");
        position.scanfVector(in);
        direction.scanfVector(in);
        stepX.scanfVector(in);
        stepX = stepX.normed();
        stepY = this->stepX % (direction.normed());
        fscanf(in, "%d %d", &height, &width);
        fclose(in);
    }

    Vector getPixel(myFloat x, myFloat y) const {
        return position + direction + stepX * x + stepY * y;
    }

    Image view(const Scene *scene) {
        Image result(width, height);
        for (int y = -height / 2; y < height / 2; ++y) {
            for (int x = -width / 2; x < width / 2; ++x) {
                result(height / 2 + y, width / 2 + x)
                    = scene->color(Ray(getPosition(),
                                       getPixel(x, y), START_POINT));


            }
        }
        return result;
    }
};









#endif
