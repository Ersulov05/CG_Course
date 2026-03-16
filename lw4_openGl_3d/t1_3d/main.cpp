#include <iostream>
#include "./src/view/Canvas/Canvas3D.h"
#include "./src/view/Canvas/ICanvas3D.h"
#include "./src/view/LargeStellateDodecahedron.h"

int main()
{
    Canvas3D canvas(800, 600);
    LargeStellateDodecahedron d;

    canvas.Run(
        [&d](ICanvas3D &canvas, float deltatime)
        {
            d.Draw(canvas);
        });

    return 0;
}