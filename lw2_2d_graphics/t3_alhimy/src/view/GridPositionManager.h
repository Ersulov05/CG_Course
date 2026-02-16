#pragma once
#include "./Common/Point.h"
#include "./Common/Size.h"
#include "./Constants.h"

class GridPositionManager
{
public:
    GridPositionManager(float startX, float startY)
        : m_startX(startX), m_startY(startY)
    {
    }

    Point GetNext()
    {
        int row = m_currentIndex / Constants::GRID_COLS;
        int col = m_currentIndex % Constants::GRID_COLS;

        if (row >= m_rows)
        {
            ++m_rows;
        }

        Point position(
            m_startX + col * (Constants::ELEMENT_SIZE.width + Constants::GRID_SPACING),
            m_startY + row * (Constants::ELEMENT_SIZE.height + Constants::GRID_SPACING));

        m_currentIndex++;
        return position;
    }

    Point GetPositionForIndex(int index) const
    {
        int row = index / Constants::GRID_COLS;
        int col = index % Constants::GRID_COLS;

        return Point(
            m_startX + col * (Constants::ELEMENT_SIZE.width + Constants::GRID_SPACING),
            m_startY + row * (Constants::ELEMENT_SIZE.height + Constants::GRID_SPACING));
    }

    void Reset()
    {
        m_currentIndex = 0;
    }

    int GetCurrentIndex() const
    {
        return m_currentIndex;
    }

private:
    int m_currentIndex = 0;
    float m_startX;
    float m_startY;
    int m_rows = Constants::GRID_ROWS;
};
