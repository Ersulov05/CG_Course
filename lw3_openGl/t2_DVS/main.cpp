#include <iostream>
#include "./src/view/Canvas/Canvas.h"
#include "./src/view/Canvas/ICanvas.h"
#include "./src/view/Common/Point.h"
#include "./src/view/DVS/DVS.h"

const unsigned WIDTH = 800;
const unsigned HEIGHT = 1000;
const Color WHITE_COLOR = Color(0xFFFFFFFF);

int main()
{
    Canvas canvas(WIDTH, HEIGHT);
    DVS dvs;

    canvas.RunWindow(
        [&dvs](ICanvas &canvas, float deltatime)
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

            canvas.PushMatrix();
            canvas.Translate(400, 700);
            canvas.Scale(2);

            dvs.Update(deltatime);
            dvs.Draw(canvas);

            canvas.PopMatrix();
        });

    return 0;
}