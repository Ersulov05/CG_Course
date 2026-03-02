#pragma once
#include "./Canvas/ICanvas.h"
#include "../model/Game.h"
#include "./EndGamePopup/EndGamePopup.h"

class UI
{
public:
    UI(Game &game, ICanvas &canvas)
        : m_game(game), m_endGamePopup(canvas, game)
    {
        game.OnEndGameSubscribe(
            this,
            [this]()
            {
                m_endGamePopup.Open();
            });
    }

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.ResetTransform();
        canvas.SetColor(0xFFFFFFFF);
        canvas.DrawText("Score:  " + std::to_string(m_game.GetScore()), {10, 24}, 24);
        DrawSpeed(canvas);
        DrawHealth(canvas);
        m_endGamePopup.Draw();
        canvas.PopMatrix();
        }

private:
    Game &m_game;
    EndGamePopup m_endGamePopup;

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