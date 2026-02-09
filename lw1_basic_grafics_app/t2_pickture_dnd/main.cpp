#include "./src/model/scene/Scene.h"
#include "./src/controller/AppController.h"
#include "./src/view/scene/SceneView.h"
#include "./src/model/common/Point.h"
#include <iostream>

const Color HOUSE_WALL_COLOR = 0xFFE4B5FF;
const Color HOUSE_ROOF_COLOR = 0x8B4513FF;
const Color HOUSE_DOOR_COLOR = 0x8B4513FF;
const Color DOOR_HANDLE_COLOR = 0xFFD700FF;
const Color HOUSE_WINDOW_COLOR = 0x87CEEBFF;
const Color CHIMNEY_COLOR = 0x696969FF;
const Color SMOKE_COLOR = 0xF5F5F5FF;
const Color FENCE_COLOR = 0x8B4513FF;
const Color GRASS_COLOR = 0x32CD32FF;
const Color SKY_COLOR = 0x87CEEBFF;
const Color CLOUD_COLOR = 0xFFFFFFFF;
const Color SUN_COLOR = 0xFFD700FF;
const Color TREE_TRUNK_COLOR = 0x8B4513FF;
const Color TREE_CROWN_COLOR = 0x228B22FF;
const Color PATH_COLOR = 0xD2B48CFF;
const Color FLOWER_COLOR = 0xFF69B4FF;
const Color BLACK_COLOR = 0x000000FF;

void DrawCountryHouse(AppController &controller, unsigned pictureId)
{
    controller.AddRectangleToPicture(pictureId, 0, 0, 800, 400, SKY_COLOR);
    controller.AddRectangleToPicture(pictureId, 700, 50, 60, 60, SUN_COLOR);

    controller.AddRectangleToPicture(pictureId, 100, 80, 100, 35, CLOUD_COLOR);
    controller.AddRectangleToPicture(pictureId, 130, 60, 100, 35, CLOUD_COLOR);

    controller.AddRectangleToPicture(pictureId, 500, 120, 90, 30, CLOUD_COLOR);
    controller.AddRectangleToPicture(pictureId, 520, 100, 90, 30, CLOUD_COLOR);

    controller.AddRectangleToPicture(pictureId, 0, 400, 800, 200, GRASS_COLOR);

    controller.AddRectangleToPicture(pictureId, 300, 250, 200, 150, HOUSE_WALL_COLOR);
    controller.AddTriangleToPicture(pictureId, Point(250, 250), Point(550, 250), Point(400, 150), HOUSE_ROOF_COLOR);

    controller.AddRectangleToPicture(pictureId, 450, 170, 30, 40, CHIMNEY_COLOR);
    controller.AddRectangleToPicture(pictureId, 445, 160, 40, 10, CHIMNEY_COLOR);
    controller.AddRectangleToPicture(pictureId, 455, 140, 15, 20, SMOKE_COLOR);
    controller.AddRectangleToPicture(pictureId, 450, 130, 25, 15, SMOKE_COLOR);

    controller.AddRectangleToPicture(pictureId, 370, 330, 60, 70, HOUSE_DOOR_COLOR);
    controller.AddRectangleToPicture(pictureId, 415, 365, 5, 5, DOOR_HANDLE_COLOR);

    controller.AddRectangleToPicture(pictureId, 320, 280, 50, 50, HOUSE_WINDOW_COLOR);
    controller.AddRectangleToPicture(pictureId, 345, 280, 2, 50, BLACK_COLOR);
    controller.AddRectangleToPicture(pictureId, 320, 305, 50, 2, BLACK_COLOR);

    controller.AddRectangleToPicture(pictureId, 100, 350, 15, 50, TREE_TRUNK_COLOR);
    controller.AddTriangleToPicture(pictureId, Point(80, 390), Point(135, 390), Point(107.5, 340), TREE_CROWN_COLOR);
    controller.AddTriangleToPicture(pictureId, Point(85, 360), Point(130, 360), Point(107.5, 320), TREE_CROWN_COLOR);
    controller.AddRectangleToPicture(pictureId, 650, 360, 20, 40, TREE_TRUNK_COLOR);
    controller.AddTriangleToPicture(pictureId, Point(630, 380), Point(690, 380), Point(660, 330), TREE_CROWN_COLOR);
    controller.AddTriangleToPicture(pictureId, Point(635, 350), Point(685, 350), Point(660, 300), TREE_CROWN_COLOR);
}

int main()
{
    Scene scene;
    AppController controller(scene);
    auto newPictureId = controller.CreatePicture();
    DrawCountryHouse(controller, newPictureId);
    SceneView sceneView(controller, scene);
    sceneView.Run();

    return 0;
}