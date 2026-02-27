#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"
#include <vector>

class BodyDVS
{
public:
    BodyDVS()
    {
    }

    void Draw(ICanvas &canvas)
    {
        canvas.SetColor(Color(0xAFAFAFFF));
        canvas.FillRect({-22, -190}, {44, 20});

        canvas.SetColor(Color(0x000000FF));
        canvas.DrawRect({-22, -190}, {44, 20}, 3);

        canvas.PushMatrix();
        canvas.Translate(-60, 0);
        canvas.Scale(-1, 1);
        DrawOuterDVSCorpus(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(60, 0);
        DrawOuterDVSCorpus(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(-29, -160.5);
        canvas.Rotate(-27);
        DrawValveContainer(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(29, -160.5);
        canvas.Rotate(27);
        DrawValveContainer(canvas);
        canvas.PopMatrix();

        DrawCilinder(canvas);

        DrawCarter(canvas);
    }

private:
    Point m_position;
    float m_scale;

    void DrawCilinder(ICanvas &canvas)
    {
        std::vector<Point> cilinder = {
            {-6, -172},
            {-18, -166},
            {-42, -190},
            {-105, -190},
            {-105, -175},
            {-60, -175},
            {-40, -155},

            {-50, -150},
            {-50, 0},
            {50, 0},
            {50, -150},

            {40, -155},
            {60, -175},
            {105, -175},
            {105, -190},
            {42, -190},
            {18, -166},
            {6, -172}};

        canvas.SetColor(Color(0x00779FFF));
        canvas.FillPolygon(cilinder);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(cilinder, 3);
    }

    void DrawCarter(ICanvas &canvas)
    {
        std::vector<Point> carter = {
            {-50, -60},
            {-65, -35},
            {-80, 0},
            {-80, 50},
            {-60, 60},
            {60, 60},
            {80, 50},
            {80, 0},
            {65, -35},
            {50, -60}};

        canvas.SetColor(Color(0x4D4D4DFF));
        canvas.FillPolygon(carter);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(carter, 3, false);
    }

    void DrawValveContainer(ICanvas &canvas)
    {
        std::vector<Point> valveContainer = {
            {-25, 0},
            {-25, -75},
            {-20, -80},
            {20, -80},
            {25, -75},
            {25, 0}};

        std::vector<Point> valveOuterContainer = {
            {-25, 0},
            {-25, -55},
            {-10, -55},
            {-10, -45},
            {10, -45},
            {10, -55},
            {25, -55},
            {25, 0}};

        canvas.SetColor(Color(0x00779FFF));
        canvas.FillPolygon(valveContainer);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(valveContainer, 4);

        canvas.SetColor(Color(0xAFAFAFFF));
        canvas.FillPolygon(valveOuterContainer);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(valveOuterContainer, 3);
    }

    void DrawOuterDVSCorpus(ICanvas &canvas)
    {
        canvas.SetColor(Color(0xAFAFAFFF));
        canvas.FillRect({-10, -150}, {20, 150});
        canvas.FillRect({-20, -180}, {30, 30});

        canvas.SetColor(Color(0x000000FF));
        canvas.DrawRect({-10, -150}, {20, 150}, 3);
        canvas.DrawRect({-20, -180}, {30, 30}, 3);

        canvas.SetColor(Color(0xAFAFAFFF));
        canvas.FillRect({-20, -195}, {65, 24});
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawRect({-20, -195}, {65, 24}, 3);

        canvas.SetColor(Color(0x00FF00FF));
        canvas.FillCircle({0, -140}, 4);
        canvas.FillCircle({0, -70}, 4);
        canvas.FillRect({-4, -140}, {8, 70});
    }
};