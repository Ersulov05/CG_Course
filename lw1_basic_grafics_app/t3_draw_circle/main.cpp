#include "./src/Canvas/CCanvas.h"
#include "./src/Common/Color.h"
#include <iostream>

const Color RED_COLOR = 0xFF0000FF;
const unsigned WIDTH = 800;
const unsigned HEIGHT = 600;

int main()
{
    try
    {
        CCanvas canvas(WIDTH, HEIGHT);
        canvas.RunWindow(
            [&](ICanvas &canvasRef, float deltaTime)
            {
                canvasRef.SetColor(RED_COLOR);
                canvasRef.DrawCircle(Point(150, 150), 100, 50);
                canvasRef.DrawCircle(Point(450, 150), 100, 50, true);

                canvasRef.FillCircle(Point(150, 450), 100);
                canvasRef.FillCircle(Point(450, 450), 100, true);
            });
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}