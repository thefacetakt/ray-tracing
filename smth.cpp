#include "rendering/Image.hpp"
#include "figures/Triangle.hpp"
#include "scene/Camera.hpp"
#include "scene/Scene.hpp"
#include "reading/STLReader.hpp"
#include "containers/Container.hpp"
#include "containers/StupidContainer.hpp"

int main(int argc, char **argv) {
    Camera camera(Vector(0, 0, 0),
                  Vector(0, 0, 50),
                  Vector(1, 0, 0));
    char *lampsFile = argv[1];
    char *sceneFile = argv[2];
    char *outFile = argv[3];
    Scene *scene = new Scene(lampsFile, sceneFile);
    Image image = view(camera, scene, 480, 720);
    image.write(outFile);
    delete scene;
    return 0;
}
