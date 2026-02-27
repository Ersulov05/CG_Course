#include <iostream>
#include "./src/view/Canvas/Canvas.h"
#include "./src/view/Canvas/ICanvas.h"
#include "./src/view/Common/Point.h"
#include "./src/view/DVS/DVS.h"
#include <chrono>

const unsigned WIDTH = 800;
const unsigned HEIGHT = 1000;
const Color WHITE_COLOR = Color(0xFFFFFFFF);

int main()
{
    Canvas canvas(WIDTH, HEIGHT);
    DVS dvs;

    int frameCount = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();

    canvas.RunWindow(
        [&dvs, &frameCount, &lastTime](ICanvas &canvas)
        {
            canvas.PushMatrix();
            canvas.Translate(400, 700);
            canvas.Scale(2);

            dvs.Update();
            // dvs.Draw(canvas);

            for (int i = 0; i < 100; i++)
            { // Искусственная нагрузка
                // dvs.Update();
                dvs.Draw(canvas);
            }

            canvas.PopMatrix();

            frameCount++;
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
            if (duration.count() >= 1)
            {
                std::cout << "FPS: " << frameCount << std::endl;
                frameCount = 0;
                lastTime = now;
            }
        });

    return 0;
}