#include "rendering/Image.hpp"
#include "figures/Triangle.hpp"
#include "scene/Camera.hpp"
#include "scene/Scene.hpp"
#include "reading/STLReader.hpp"
#include "containers/Container.hpp"
#include "containers/StupidContainer.hpp"

int main(int argc, char **argv) {
    Camera camera(Vector(-150, 100, 200),
                  Vector(3000, 0, 0),
                  Vector(0, 0, 1));
    char *lampsFile = argv[1];
    char *sceneFile = argv[2];
    char *outFile = argv[3];
    Scene *scene = new Scene(lampsFile, sceneFile);
    Image image = view(camera, scene, 2160, 3840);
    image.write(outFile);
    delete scene;
    printf("%d\n", steps);
    return 0;
}
