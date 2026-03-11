#pragma once
#include "../../common/TransformMatrix.h"
#include <stack>

class Transformable
{
public:
    virtual ~Transformable() = default;

    void PushMatrix()
    {
        m_matrixStack.push(m_currentTransform);
    }

    void PopMatrix()
    {
        if (!m_matrixStack.empty())
        {
            m_currentTransform = m_matrixStack.top();
            m_matrixStack.pop();
        }
    }

    void Translate(float x, float y)
    {
        m_currentTransform.Translate(x, y);
    }

    void Translate(const Point &p)
    {
        m_currentTransform.Translate(p);
    }

    void Rotate(float angleDegrees)
    {
        m_currentTransform.Rotate(angleDegrees);
    }

    void Scale(float scaleX, float scaleY)
    {
        m_currentTransform.Scale(scaleX, scaleY);
    }

    void Scale(float scale)
    {
        m_currentTransform.Scale(scale);
    }

    void Scale(const Point &scale)
    {
        m_currentTransform.Scale(scale);
    }

    void ResetTransform()
    {
        m_currentTransform.Reset();
    }

    const TransformMatrix &GetTransform() const
    {
        return m_currentTransform;
    }

    void SetTransform(const TransformMatrix &transform)
    {
        m_currentTransform = transform;
    }

protected:
    TransformMatrix m_currentTransform;
    std::stack<TransformMatrix> m_matrixStack;
};