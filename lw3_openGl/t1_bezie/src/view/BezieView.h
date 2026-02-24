#pragma once
#include "./PointView.h"
#include "./Canvas/ICanvas.h"
#include "./Common/Point.h"

const float STRIP_SIZE = 10;

class BezieView
{
public:
    BezieView(ICanvas &canvas, const Point &p0, const Point &p1, const Point &p2, const Point &p3)
        : m_canvas(canvas),
          m_startPoint(canvas, p0),
          m_firstPoint(canvas, p1),
          m_secondPoint(canvas, p2),
          m_endPoint(canvas, p3)
    {
    }

    void Draw()
    {
        m_canvas.DrawCubeBezie(
            m_startPoint.GetCenterPosition(),
            m_firstPoint.GetCenterPosition(),
            m_secondPoint.GetCenterPosition(),
            m_endPoint.GetCenterPosition());
        m_canvas.DrawStripLine(m_startPoint.GetCenterPosition(), m_firstPoint.GetCenterPosition(), STRIP_SIZE);
        m_canvas.DrawStripLine(m_firstPoint.GetCenterPosition(), m_secondPoint.GetCenterPosition(), STRIP_SIZE);
        m_canvas.DrawStripLine(m_secondPoint.GetCenterPosition(), m_endPoint.GetCenterPosition(), STRIP_SIZE);
        m_startPoint.Draw();
        m_firstPoint.Draw();
        m_secondPoint.Draw();
        m_endPoint.Draw();
    }

private:
    ICanvas &m_canvas;
    PointView m_startPoint;
    PointView m_endPoint;
    PointView m_firstPoint;
    PointView m_secondPoint;
};