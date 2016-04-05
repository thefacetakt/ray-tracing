#include "Image.hpp"
#include "figures/Triangle.hpp"
#include "Camera.hpp"
#include "STLReader.hpp"

int main() {
    Camera camera(Vector(0, 0, 0),
                  Vector(0, 0, 1),
                  Vector(1, 0, 0));
    vector <Figure *> figures = readFile("scene.stl");
    Image image = view(camera, figures, 1080, 1920);
    image.write("triangle.png");
    for (auto &figure: figures) {
        delete figure;
    }
    return 0;
}
