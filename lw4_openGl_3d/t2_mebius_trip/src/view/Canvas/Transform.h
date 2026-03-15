#pragma once
#include "../../common/TransformMatrix.h"
#include <stack>

class Transform
{
public:
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

    void Translate(float x, float y, float z)
    {
        m_currentTransform.Translate(x, y, z);
    }

    void RotateX(float angleDegrees)
    {
        m_currentTransform.RotateX(angleDegrees);
    }

    void RotateY(float angleDegrees)
    {
        m_currentTransform.RotateY(angleDegrees);
    }

    void RotateZ(float angleDegrees)
    {
        m_currentTransform.RotateZ(angleDegrees);
    }

    void Scale(float scaleX, float scaleY, float scaleZ)
    {
        m_currentTransform.Scale(scaleX, scaleY, scaleZ);
    }

    void Scale(float scale)
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

protected:
    TransformMatrix m_currentTransform;
    std::stack<TransformMatrix> m_matrixStack;
};