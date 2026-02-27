#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class ConnectingRod
{
public:
    ConnectingRod(float size)
        : m_size(size)
    {
    }

    void Draw(ICanvas &canvas)
    {
        std::vector<Point> connectingRod = {
            {-2, -m_size},
            {-2, 0},
            {2, 0},
            {2, -m_size}};

        canvas.SetColor(0x0000FFFF);
        canvas.DrawPolygon(connectingRod, 2);

        std::vector<Point> connectingDownRect = {
            {-28, 24},
            {-28, 0},
            {28, 0},
            {28, 24}};

        std::vector<Point> connectingUpRect = {
            {-8, -m_size},
            {-16, -26},
            {-28, -20},
            {-28, 0},
            {28, 0},
            {28, -20},
            {16, -26},
            {8, -m_size}};

        std::vector<Point> internalConnectingDownRect = {
            {-27, 23.5},
            {-27, 1},
            {27, 1},
            {27, 23.5}};

        Point downCircleCenter = {0, 16};

        canvas.SetColor(Color(0xFFD700FF));
        canvas.FillPolygon(connectingUpRect);
        canvas.SetColor(0x000000FF);
        canvas.DrawPolygon(connectingUpRect, 3);

        canvas.SetColor(Color(0xFFD700FF));
        canvas.FillPolygon(connectingDownRect);
        canvas.SetColor(0x000000FF);
        canvas.DrawPolygon(connectingDownRect, 3);

        canvas.SetColor(Color(0xFFD700FF));
        canvas.FillCircle(downCircleCenter, 18);
        canvas.SetColor(0x000000FF);
        canvas.DrawCircle(downCircleCenter, 18, 3);

        canvas.SetColor(Color(0xFFD700FF));
        canvas.FillPolygon(internalConnectingDownRect);

        canvas.SetColor(0x8F8F8FFF);
        canvas.FillCircle({0, 0}, 22);
        canvas.SetColor(0xFFFFFFFF);
        canvas.FillCircle({0, 0}, 18);
    }

private:
    float m_size;
};