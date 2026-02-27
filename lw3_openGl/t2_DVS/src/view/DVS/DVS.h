#pragma once
#include "./Valve.h"
#include "./SparkPlug.h"
#include "./BodyDVS.h"
#include "../Canvas/ICanvas.h"
#include "./PistonRodCrank/PistonRodCrank.h"

class DVS
{
public:
    DVS()
        : m_body(),
          m_sparkPlug(), // сделать через матрицу, вращение также через матрицу
          m_intakeValve(),
          m_exhaustValve(),
          m_pistonRodCrank()
    {
    }

    void Update(float deltatime)
    {
        m_pistonRodCrank.Update(deltatime);
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

        canvas.PushMatrix();
        canvas.Translate(Point(-29, -160.5));
        canvas.Rotate(-27);
        m_intakeValve.Draw(canvas);
        canvas.PopMatrix();

        canvas.PushMatrix();
        canvas.Translate(Point(29, -160.5));
        canvas.Rotate(27);
        m_exhaustValve.Draw(canvas);
        canvas.PopMatrix();

        m_pistonRodCrank.Draw(canvas);

        canvas.PushMatrix();
        canvas.Translate(Point(0, -184));
        m_sparkPlug.Draw(canvas);
        canvas.PopMatrix();
    }

private:
    Valve m_intakeValve;
    Valve m_exhaustValve;
    SparkPlug m_sparkPlug;
    BodyDVS m_body;
    PistonRodCrank m_pistonRodCrank;
};