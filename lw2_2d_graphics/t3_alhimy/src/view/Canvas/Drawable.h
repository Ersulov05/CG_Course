#pragma once

class Drawable
{
public:
    virtual void Draw(unsigned &zIndex)
    {
        m_zIndex = zIndex++;
    }

    unsigned GetZIndex()
    {
        return m_zIndex;
    }

private:
    unsigned m_zIndex;
};