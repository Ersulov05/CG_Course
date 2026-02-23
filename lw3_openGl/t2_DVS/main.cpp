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
    DVS dvs({WIDTH / 2, HEIGHT / 2 + 100}, 2);

    canvas.RunWindow(
        [&dvs](ICanvas &canvas)
        {
            dvs.Update();
            dvs.Draw(canvas);
            // canvas.SetColor(0xFFFFFFFF);
            // canvas.FillPolygon({{80, 100}, {200, 100}, {200, 200}, {300, 200}, {300, 320}, {200, 320}});
            // canvas.SetColor(0x0000FFFF);
            // canvas.DrawPolygon({{80, 100}, {200, 100}, {200, 200}, {300, 200}, {300, 320}, {200, 320}});
        });

    return 0;
}