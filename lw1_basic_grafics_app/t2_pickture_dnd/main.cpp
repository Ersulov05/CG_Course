#include "./src/model/scene/Scene.h"
#include "./src/controller/AppController.h"
#include "./src/view/scene/SceneView.h"
#include "./src/model/common/Point.h"
#include <iostream>

const Color RED_COLOR = 0xFF0000FF;
const Color BLUE_COLOR = 0x0000FFFF;
// const unsigned WIDTH = 800;
// const unsigned HEIGHT = 600;

int main()
{
    Scene scene;
    AppController controller(scene);
    auto newPictureId = controller.CreatePicture();
    controller.AddRectangleToPicture(newPictureId, 100, 100, 300, 300, RED_COLOR);
    controller.AddRectangleToPicture(newPictureId, 200, 200, 100, 100, BLUE_COLOR);
    controller.AddTriangleToPicture(newPictureId, Point(100, 100), Point(400, 100), Point(250, 0));
    SceneView sceneView(controller, scene);
    sceneView.Run();

    return 0;
}