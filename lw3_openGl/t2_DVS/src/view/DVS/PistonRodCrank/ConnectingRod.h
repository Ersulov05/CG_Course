#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class ConnectingRod
{
public:
    ConnectingRod(const Point &position, float size, float scale)
        : m_position(position),
          m_size(size),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        std::vector<Point> baseConnectingRod = {
            {x - 2, y - m_size},
            {x - 2, y},
            {x + 2, y},
            {x + 2, y - m_size}};

        std::vector<Point> connectingRod = TransformPoints(baseConnectingRod, m_position, m_rotation, m_scale);

        canvas.SetColor(0xFF0000FF);
        canvas.DrawPolygon(connectingRod, 2);

        std::vector<Point> baseConnectingDownRect = {
            {x - 28, y + 24},
            {x - 28, y},
            {x + 28, y},
            {x + 28, y + 24}};

        std::vector<Point> baseConnectingUpRect = {
            {x - 8, y - m_size},
            {x - 16, y - 26},
            {x - 28, y - 20},
            {x - 28, y},
            {x + 28, y},
            {x + 28, y - 20},
            {x + 16, y - 26},
            {x + 8, y - m_size}};

        std::vector<Point> baseInternalConnectingDownRect = {
            {x - 27, y + 23.5},
            {x - 27, y + 1},
            {x + 27, y + 1},
            {x + 27, y + 23.5}};

        std::vector<Point> connectingDownRect = TransformPoints(baseConnectingDownRect, m_position, m_rotation, m_scale);
        std::vector<Point> connectingUpRect = TransformPoints(baseConnectingUpRect, m_position, m_rotation, m_scale);
        std::vector<Point> internalConnectingDownRect = TransformPoints(baseInternalConnectingDownRect, m_position, m_rotation, m_scale);
        Point downCircleCenter = TransformPoint({x, y + 16}, m_position, m_rotation, m_scale);

        canvas.SetColor(Color(0xFFFFD700));
        canvas.FillPolygon(connectingUpRect);
        canvas.SetColor(0xFF000000);
        canvas.DrawPolygon(connectingUpRect, 3);

        canvas.SetColor(Color(0xFFFFD700));
        canvas.FillPolygon(connectingDownRect);
        canvas.SetColor(0xFF000000);
        canvas.DrawPolygon(connectingDownRect, 3);

        canvas.SetColor(Color(0xFFFFD700));
        canvas.FillCircle(downCircleCenter, 18 * m_scale);
        canvas.SetColor(0xFF000000);
        canvas.DrawCircle(downCircleCenter, 18 * m_scale, 3);

        canvas.SetColor(Color(0xFFFFD700));
        canvas.FillPolygon(internalConnectingDownRect);

        canvas.SetColor(0xFF8F8F8F);
        canvas.FillCircle(m_position, 22 * m_scale);
        canvas.SetColor(0xFFFFFFFF);
        canvas.FillCircle(m_position, 18 * m_scale);
    }

    void
    SetPosition(const Point &position)
    {
        m_position = position;
    }

    void SetRotation(float rotation)
    {
        m_rotation = rotation;
    }

private:
    Point m_position;
    float m_rotation;
    float m_scale;
    float m_size;
};