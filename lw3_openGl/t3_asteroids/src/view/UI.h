#pragma once
#include "./Canvas/ICanvas.h"
#include "../model/Game.h"

class UI
{
public:
    UI(Game &game) : m_game(game)
    {
    }

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.ResetTransform();
        DrawSpeed(canvas);
        DrawHealth(canvas);
        canvas.PopMatrix();
    }

private:
    Game &m_game;

    void DrawSpeed(ICanvas &canvas)
    {
        auto &spaceshipModel = m_game.GetSpaceshipModel();
        auto maxSpeed = spaceshipModel.MAX_SPEED;
        auto speed = spaceshipModel.GetSpeed();
        float speedCoef = speed / maxSpeed;

        canvas.PushMatrix();
        canvas.Translate(canvas.GetWidth() - 25, canvas.GetHeight() - 95);
        canvas.Scale(1, -1);
        canvas.Translate(0, -75);

        canvas.SetColor(0x34495EFF);
        canvas.FillRect({-10, 0}, {20, 150});
        canvas.SetColor(0xF39C12FF);
        canvas.FillRect({-10, 0}, {20, 150 * speedCoef});
        canvas.SetColor(0xECF0F1FF);
        canvas.DrawRect({-10, 0}, {20, 150}, 3);

        canvas.PopMatrix();
    }

    void DrawHealth(ICanvas &canvas)
    {
        auto &spaceshipModel = m_game.GetSpaceshipModel();
        auto healthCoef = (float)spaceshipModel.GetHealth() / spaceshipModel.MAX_HEALTH;

        canvas.PushMatrix();
        canvas.Translate(20, canvas.GetHeight() - 40);

        canvas.SetColor(0x34495EFF);
        canvas.FillRect({0, 0}, {150, 20});
        if (spaceshipModel.IsProtected())
        {
            canvas.SetColor(0x00CCCCFF);
        }
        else
        {
            canvas.SetColor(0xE74C3CFF);
        }

        canvas.FillRect({0, 0}, {150 * healthCoef, 20});
        canvas.SetColor(0xECF0F1FF);
        canvas.DrawRect({0, 0}, {150, 20}, 3);

        canvas.PopMatrix();
    }
};