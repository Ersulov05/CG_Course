#include "./src/Canvas/CCanvas.h"
#include "./src/Letter/LetterStrategy/LetterAStrategy.h"
#include "./src/Letter/LetterStrategy/LetterEStrategy.h"
#include "./src/Letter/MovementStrategy/JumpStrategy.h"
#include "./src/Letter/Letter.h"
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
        auto letterAStrategy = std::make_unique<LetterAStrategy>();
        auto letterEStrategy = std::make_unique<LetterEStrategy>();
        auto jumpEStrategy = std::make_unique<JumpStrategy>(-200);
        auto jumpA1Strategy = std::make_unique<JumpStrategy>(-100);
        auto jumpA2Strategy = std::make_unique<JumpStrategy>(0);

        Letter letterE(std::move(letterEStrategy), std::move(jumpEStrategy), RED_COLOR, {100, 300});
        Letter letterA1(std::move(letterAStrategy->Clone()), std::move(jumpA1Strategy), RED_COLOR, {250, 300});
        Letter letterA2(std::move(letterAStrategy), std::move(jumpA2Strategy), RED_COLOR, {400, 300});

        canvas.RunWindow(
            [&](ICanvas &canvasRef, float deltaTime)
            {
                letterE.Update(deltaTime);
                letterA1.Update(deltaTime);
                letterA2.Update(deltaTime);

                canvas.Clear(0x1A1A1AFF);
                letterE.Draw(canvas);
                letterA1.Draw(canvas);
                letterA2.Draw(canvas);
                canvas.Display();
            });
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}