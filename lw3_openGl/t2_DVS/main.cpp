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
        });

    return 0;
}