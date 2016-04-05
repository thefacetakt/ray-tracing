#include "Image.hpp"
#include "figures/Triangle.hpp"
#include "Camera.hpp"
#include "STLReader.hpp"
#include "containers/Container.hpp"
#include "containers/StupidContainer.hpp"

int main() {
    Camera camera(Vector(0, 0, 0),
                  Vector(0, 0, 1),
                  Vector(1, 0, 0));
    Container *container = dynamic_cast<Container *>(
        new StupidContainer("scene.stl"));
    Image image = view(camera, container, 1080, 1920);
    image.write("triangle.png");
    delete container;
    return 0;
}
