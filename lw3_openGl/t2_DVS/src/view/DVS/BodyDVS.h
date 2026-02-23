#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"
#include <vector>

class BodyDVS
{
public:
    BodyDVS(const Point &position, float scale)
        : m_position(position),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {

        canvas.SetColor(Color(0xFFAFAFAF));
        canvas.FillRect({m_position.x - 22 * m_scale, m_position.y - 190 * m_scale}, {44 * m_scale, 20 * m_scale});

        canvas.SetColor(Color(0xFF000000));
        canvas.DrawRect({m_position.x - 22 * m_scale, m_position.y - 190 * m_scale}, {44 * m_scale, 20 * m_scale}, 3);
        DrawOuterDVSCorpus(canvas, {m_position.x - 60 * m_scale, m_position.y}, true);
        DrawOuterDVSCorpus(canvas, {m_position.x + 60 * m_scale, m_position.y});
        DrawValveContainer(canvas, {m_position.x - 29 * m_scale, m_position.y - 160.5 * m_scale}, -27);
        DrawValveContainer(canvas, {m_position.x + 29 * m_scale, m_position.y - 160.5 * m_scale}, 27);
        DrawCilinder(canvas);
        DrawCarter(canvas);
        // DrawOuterDVSCorpus(canvas, {m_position.x - 60 * m_scale, m_position.y}, true);
        // DrawOuterDVSCorpus(canvas, {m_position.x + 60 * m_scale, m_position.y});
    }

private:
    Point m_position;
    float m_scale;

    void DrawCilinder(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        std::vector<Point> cilinder = {
            {x - 6 * m_scale, y - 172 * m_scale},
            {x - 18 * m_scale, y - 166 * m_scale},
            {x - 42 * m_scale, y - 190 * m_scale},
            {x - 105 * m_scale, y - 190 * m_scale},
            {x - 105 * m_scale, y - 175 * m_scale},
            {x - 60 * m_scale, y - 175 * m_scale},
            {x - 40 * m_scale, y - 155 * m_scale},

            {x - 50 * m_scale, y - 150 * m_scale},
            {x - 50 * m_scale, y},
            {x + 50 * m_scale, y},
            {x + 50 * m_scale, y - 150 * m_scale},

            {x + 40 * m_scale, y - 155 * m_scale},
            {x + 60 * m_scale, y - 175 * m_scale},
            {x + 105 * m_scale, y - 175 * m_scale},
            {x + 105 * m_scale, y - 190 * m_scale},
            {x + 42 * m_scale, y - 190 * m_scale},
            {x + 18 * m_scale, y - 166 * m_scale},
            {x + 6 * m_scale, y - 172 * m_scale}};

        canvas.SetColor(Color(0xFF00779F));
        canvas.FillPolygon(cilinder);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(cilinder, 3);
    }

    void DrawCarter(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        std::vector<Point> baseCarter = {
            {x - 50, y - 60},
            {x - 65, y - 35},
            {x - 80, y},
            {x - 80, y + 50},
            {x - 60, y + 60},
            {x + 60, y + 60},
            {x + 80, y + 50},
            {x + 80, y},
            {x + 65, y - 35},
            {x + 50, y - 60}};

        auto carter = TransformPoints(baseCarter, m_position, 0, m_scale);

        canvas.SetColor(Color(0xFF4D4D4D));
        canvas.FillPolygon(carter);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(carter, 3, false);
    }

    void DrawValveContainer(ICanvas &canvas, const Point &position, float rotation)
    {
        auto x = position.x;
        auto y = position.y;

        std::vector<Point> baseValveContainer = {
            {x - 25, y},
            {x - 25, y - 75},
            {x - 20, y - 80},
            {x + 20, y - 80},
            {x + 25, y - 75},
            {x + 25, y}};

        std::vector<Point> baseValveOuterContainer = {
            {x - 25, y},
            {x - 25, y - 55},
            {x - 10, y - 55},
            {x - 10, y - 45},
            {x + 10, y - 45},
            {x + 10, y - 55},
            {x + 25, y - 55},
            {x + 25, y}};

        auto valveContainer = TransformPoints(baseValveContainer, position, rotation, m_scale);
        auto valveOuterContainer = TransformPoints(baseValveOuterContainer, position, rotation, m_scale);

        canvas.SetColor(Color(0xFF00779F));
        canvas.FillPolygon(valveContainer);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(valveContainer, 4);

        canvas.SetColor(Color(0xFFAFAFAF));
        canvas.FillPolygon(valveOuterContainer);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(valveOuterContainer, 3);
    }

    void DrawOuterDVSCorpus(ICanvas &canvas, const Point &position, bool isMirror = false)
    {
        canvas.SetColor(Color(0xFFAFAFAF));
        canvas.FillRect({position.x - 10 * m_scale, position.y - 150 * m_scale}, {20 * m_scale, 150 * m_scale});
        canvas.FillRect({position.x - (10 + (isMirror ? 0 : 10)) * m_scale, position.y - 180 * m_scale}, {30 * m_scale, 30 * m_scale});

        canvas.SetColor(Color(0xFF000000));
        canvas.DrawRect({position.x - 10 * m_scale, position.y - 150 * m_scale}, {20 * m_scale, 150 * m_scale}, 3);
        canvas.DrawRect({position.x - (10 + (isMirror ? 0 : 10)) * m_scale, position.y - 180 * m_scale}, {30 * m_scale, 30 * m_scale}, 3);

        canvas.SetColor(Color(0xFFAFAFAF));
        canvas.FillRect({position.x - (10 + (isMirror ? 35 : 10)) * m_scale, position.y - 195 * m_scale}, {65 * m_scale, 24 * m_scale});
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawRect({position.x - (10 + (isMirror ? 35 : 10)) * m_scale, position.y - 195 * m_scale}, {65 * m_scale, 24 * m_scale}, 3);

        canvas.SetColor(Color(0xFF00FF00));
        canvas.FillCircle({position.x, position.y - 140 * m_scale}, 4 * m_scale);
        canvas.FillCircle({position.x, position.y - 70 * m_scale}, 4 * m_scale);
        canvas.FillRect({position.x - 8, position.y - 140 * m_scale}, {8 * m_scale, 70 * m_scale});
    }
};