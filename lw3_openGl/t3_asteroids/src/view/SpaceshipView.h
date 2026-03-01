#pragma once
#include "../model/SpaceshipModel.h"
#include "./Canvas/ICanvas.h"

class SpaceshipView
{
public:
    SpaceshipView(SpaceshipModel &spaceshipModel) : m_spaceshipModel(spaceshipModel) {}

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.Rotate(m_spaceshipModel.GetRotation());

        DrawCorpus(canvas);
        DrawIlluminator(canvas);
        DrawEngine(canvas);

        if (m_spaceshipModel.IsEngineOn())
        {
            DrawFire(canvas);
        }

        if (m_spaceshipModel.IsProtected())
        {
            DrawProtect(canvas);
        }

        canvas.PopMatrix();
    }

    Point GetPosition()
    {
        return m_spaceshipModel.GetPosition();
    }

private:
    SpaceshipModel &m_spaceshipModel;
    const float WIDTH = 20;
    const float HEIGHT = 40;
    const std::vector<Point> CORPUS = {
        {-WIDTH * 0.4, -HEIGHT / 2},
        {-WIDTH / 2, HEIGHT / 4},
        {-WIDTH / 3, HEIGHT / 2},
        {WIDTH / 3, HEIGHT / 2},
        {WIDTH / 2, HEIGHT / 4},
        {WIDTH * 0.4, -HEIGHT / 2}};

    void DrawCorpus(ICanvas &canvas)
    {
        canvas.SetColor(0x3498DBFF);
        canvas.FillPolygon(CORPUS);
        canvas.SetColor(0x2980B9FF);
        canvas.DrawPolygon(CORPUS, 2.0f);
    }

    void DrawIlluminator(ICanvas &canvas)
    {
        canvas.SetColor(0xE0E0E0FF);
        canvas.FillCircle({0, -HEIGHT / 5}, WIDTH / 4);
    }

    void DrawEngine(ICanvas &canvas)
    {
        canvas.SetColor(0xE67E22FF);
        canvas.FillRect({-WIDTH / 3, HEIGHT / 3}, {WIDTH / 6, HEIGHT / 6});
        canvas.FillRect({WIDTH / 6, HEIGHT / 3}, {WIDTH / 6, HEIGHT / 6});
    }

    void DrawFire(ICanvas &canvas)
    {
        canvas.SetColor(0xF39C12FF);
        canvas.FillPolygon({{-WIDTH / 4, HEIGHT / 2 + 4},
                            {WIDTH / 4, HEIGHT / 2 + 4},
                            {0, HEIGHT / 2 + 20}});
    }

    void DrawProtect(ICanvas &canvas)
    {
        canvas.SetColor(0x00CCCCFF);
        canvas.DrawCircle({0, 0}, m_spaceshipModel.SPACESHIP_PROTECTION_RADIUS, 3);
    }
};