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
    PistonRodCrank(const Point &position, float scale)
        : m_position(position),
          m_scale(scale),
          m_piston(Piston({position.x, position.y - CONNECTING_ROD_SIZE * scale}, scale)),
          m_connectingRod(ConnectingRod({position.x + RADIUS * scale, position.y}, CONNECTING_ROD_SIZE, scale)),
          m_crankshaft(Crankshaft({position.x, position.y}, RADIUS, 90, scale))
    {
        m_piston.SetPosition({m_position.x, m_position.y + GetPistonOffset() - CONNECTING_ROD_SIZE * m_scale});
        m_connectingRod.SetPosition(m_position + GetConnectingRodPositionOffset());
        m_connectingRod.SetRotation(GetConnectingRodRotation());
        m_crankshaft.SetRotation(m_rotate + 90);
    }

    void Update()
    {
        m_rotate += 0.5;
        if (m_rotate > 720)
        {
            m_rotate -= 720;
        }

        m_piston.SetPosition({m_position.x, m_position.y + GetPistonOffset() - CONNECTING_ROD_SIZE * m_scale});
        m_connectingRod.SetPosition(m_position + GetConnectingRodPositionOffset());
        m_connectingRod.SetRotation(GetConnectingRodRotation());
        m_crankshaft.SetRotation(m_rotate + 90);
    }

    void Draw(ICanvas &canvas)
    {
        m_crankshaft.Draw(canvas);
        m_connectingRod.Draw(canvas);
        m_piston.Draw(canvas);
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

        return (RADIUS * sin(m_rotate * M_PI / 180) + CONNECTING_ROD_SIZE * (1 - fabs(cos(angle * M_PI / 180)))) * m_scale;
    }

    Point GetConnectingRodPositionOffset()
    {
        return Point(RADIUS * cos(m_rotate * M_PI / 180) * m_scale, RADIUS * sin(m_rotate * M_PI / 180) * m_scale);
    }

    float GetConnectingRodRotation()
    {
        return -atan(RADIUS * cos(m_rotate * M_PI / 180) / CONNECTING_ROD_SIZE) * 180 / 3.1415926f;
    }
};