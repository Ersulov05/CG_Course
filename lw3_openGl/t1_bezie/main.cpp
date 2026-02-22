#include <iostream>
#include "./src/view/Canvas/Canvas.h"
#include "./src/view/Canvas/ICanvas.h"
#include "./src/view/Common/Point.h"
#include "./src/view/BezieView.h"

const unsigned WIDTH = 800;
const unsigned HEIGHT = 600;
const Color WHITE_COLOR = Color(0xFFFFFFFF);

int main()
{
    Canvas canvas(WIDTH, HEIGHT);
    BezieView bezie(canvas, {100, 100}, {100, 200}, {200, 100}, {200, 200});
    auto &mouseController = canvas.GetMouseController();

    canvas.RunWindow(
        [&bezie](ICanvas &canvas)
        {
            canvas.SetColor(WHITE_COLOR);
            bezie.Draw();
        });

    return 0;
}