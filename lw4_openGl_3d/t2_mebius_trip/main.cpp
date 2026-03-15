#include <iostream>
#include "./src/view/Canvas/Canvas3D.h"
#include "./src/view/Canvas/ICanvas3D.h"
#include "./src/view/MebiusTrip.h"

int main()
{
    Canvas3D canvas(800, 600);
    MebiusTrip ms;

    canvas.Run(
        [&ms](ICanvas3D &canvas, float deltatime)
        {
            static float fpsTimer = 0.0f;
            static int frameCount = 0;
            static int currentFPS = 0;

            frameCount++;
            fpsTimer += deltatime;

            if (fpsTimer >= 1.0f)
            {
                currentFPS = (frameCount / fpsTimer);
                std::cout << "FPS: " << currentFPS << " | Frame time: " << deltatime * 1000.0f << " ms" << std::endl;

                frameCount = 0;
                fpsTimer = 0.0f;
            }

            ms.Draw(canvas);
        });

    return 0;
}