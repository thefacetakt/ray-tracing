#ifndef RT_CAMERA
#define RT_CAMERA

#include "../rendering/Image.hpp"
#include "../figures/Figure.hpp"
#include "../containers/Container.hpp"
#include "../parallel/ThreadPool.hpp"
#include "Scene.hpp"
#include <algorithm>
#include <thread>

using std::max;
using std::vector;
using std::thread;

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
        assert(fscanf(in, "%d %d", &height, &width) == 2);
        fclose(in);
    }

    Vector getPixel(myFloat x, myFloat y) const {
        return position + direction + stepX * x + stepY * y;
    }

    Image view(const Scene *scene) {
        ThreadPool pool(thread::hardware_concurrency());
        static int cnt = 0;
        Image result(width, height);
        for (int y = -height / 2; y < height / 2; ++y) {
            for (int x = -width / 2; x < width / 2; ++x) {
                pool.submit([x, y, scene, this, &result] {
                    result(this->height / 2 + y, this->width / 2 + x) =
                        scene->color(Ray(this->getPosition(),
                                           this->getPixel(x, y), START_POINT));
                });
            }
        }
        return result;
    }
};









#endif
