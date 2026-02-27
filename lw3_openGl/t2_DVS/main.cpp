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

    canvas.RunWindow(
        [&dvs](ICanvas &canvas, float deltatime)
        {
            canvas.PushMatrix();
            canvas.Translate(400, 700);
            canvas.Scale(2);

            dvs.Update(deltatime);
            dvs.Draw(canvas);

            canvas.PopMatrix();
        });

    return 0;
}