#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"
#include "./Piston.h"
#include "./Crankshaft.h"
#include "./ConnectingRod.h"
#include <cmath>

const int RADIUS = 20;
const int CONNECTING_ROD_SIZE = 105;

class PistonRodCrank
{
public:
    PistonRodCrank()
        : m_piston(),
          m_connectingRod(CONNECTING_ROD_SIZE),
          m_crankshaft(RADIUS)
    {
    }

    void Update(float deltatime)
    {
        m_rotate += 60 * deltatime;
        if (m_rotate > 720)
        {
            m_rotate -= 720;
        }
    }

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.Rotate(m_rotate + 90);
        m_crankshaft.Draw(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(GetConnectingRodPositionOffset());
        canvas.Rotate(GetConnectingRodRotation());
        m_connectingRod.Draw(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(0, GetPistonOffset() - CONNECTING_ROD_SIZE);
        m_piston.Draw(canvas);
        canvas.PopMatrix();
    }

    float GetRotate()
    {
        return m_rotate;
    }

private:
    Point m_position;
    float m_scale;
    float m_rotate = 650;
    Piston m_piston;
    Crankshaft m_crankshaft;
    ConnectingRod m_connectingRod;

    float GetPistonOffset()
    {
        auto angle = GetConnectingRodRotation();

        return (RADIUS * sin(m_rotate * M_PI / 180) + CONNECTING_ROD_SIZE * (1 - fabs(cos(angle * M_PI / 180))));
    }

    Point GetConnectingRodPositionOffset()
    {
        return Point(RADIUS * cos(m_rotate * M_PI / 180), RADIUS * sin(m_rotate * M_PI / 180));
    }

    float GetConnectingRodRotation()
    {
        return -atan(RADIUS * cos(m_rotate * M_PI / 180) / CONNECTING_ROD_SIZE) * 180 / M_PI;
    }
};