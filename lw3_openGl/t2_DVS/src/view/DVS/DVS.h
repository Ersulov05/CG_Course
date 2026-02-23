#pragma once
#include "./Valve.h"
#include "./SparkPlug.h"
#include "./BodyDVS.h"
#include "../Canvas/ICanvas.h"
#include "./PistonRodCrank/PistonRodCrank.h"

class DVS
{
public:
    DVS(const Point &position, float scale = 1)
        : m_body(position, scale),
          m_sparkPlug({position.x, position.y - 184 * scale}, scale),
          m_intakeValve({position.x - 29 * scale, position.y - 160.5 * scale}, -27, scale),
          m_exhaustValve({position.x + 29 * scale, position.y - 160.5 * scale}, 27, scale),
          m_pistonRodCrank(position, scale)
    {
    }

    void Update()
    {
        m_pistonRodCrank.Update();
        auto rotate = m_pistonRodCrank.GetRotate();

        if (rotate >= 650 && rotate <= 670)
            m_intakeValve.SetOpen((rotate - 650) / 20.0f);
        else if (rotate >= 70 && rotate <= 90)
            m_intakeValve.SetOpen(1.0f - (rotate - 70.0f) / 20.0f);

        if (rotate >= 450 && rotate <= 470)
            m_exhaustValve.SetOpen((rotate - 450.0f) / 20.0f);
        else if (rotate >= 630 && rotate <= 650)
            m_exhaustValve.SetOpen(1.0f - (rotate - 630.0f) / 20.0f);
    }

    void Draw(ICanvas &canvas)
    {
        m_body.Draw(canvas);
        m_intakeValve.Draw(canvas);
        m_exhaustValve.Draw(canvas);

        m_pistonRodCrank.Draw(canvas);
        m_sparkPlug.Draw(canvas);
    }

private:
    Valve m_intakeValve;
    Valve m_exhaustValve;
    SparkPlug m_sparkPlug;
    BodyDVS m_body;
    PistonRodCrank m_pistonRodCrank;
};