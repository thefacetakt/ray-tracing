#include "rendering/Image.hpp"
#include "figures/Triangle.hpp"
#include "scene/Camera.hpp"
#include "scene/Scene.hpp"
#include "reading/STLReader.hpp"
#include "containers/Container.hpp"
#include "containers/StupidContainer.hpp"



int main(int argc, char **argv) {
    char *configFile = argv[1];
    char *lampsFile = argv[2];
    char *sceneFile = argv[3];
    char *outFile = argv[4];

    Camera camera(configFile);
    Scene *scene = new Scene(lampsFile, sceneFile);

    Image image = camera.view(scene);
    image.write(outFile);
    delete scene;
#ifdef RT_DEBUG
    printf("%d\n", steps);
#endif
    return 0;
}
