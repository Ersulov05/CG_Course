#pragma once

#include <vector>
#include "../shape/IShape.h"
#include <memory>
#include <unordered_map>

class Picture
{
public:
    Picture(unsigned id) : m_id(id) {}

    void AddShape(std::shared_ptr<IShape> shape)
    {
        m_shapes.push_back(shape);
    }

    void Move(float dx, float dy)
    {
        for (auto &shape : m_shapes)
        {
            shape->Move(dx, dy);
        }
    }

    std::vector<std::shared_ptr<const IShape>> GetShapes() const
    {
        std::vector<std::shared_ptr<const IShape>> result;
        result.reserve(m_shapes.size());

        for (const auto &shape : m_shapes)
        {
            result.push_back(std::static_pointer_cast<const IShape>(shape));
        }

        return result;
    }

    size_t GetShapeCount() const
    {
        return m_shapes.size();
    }

    unsigned GetId() const
    {
        return m_id;
    }

    bool IsPointInsidePicture(const Point &point) const
    {
        for (const auto &shape : m_shapes)
        {
            if (shape->IsPointInsideShape(point))
            {
                return true;
            }
        }

        return false;
    }

private:
    unsigned m_id;
    std::vector<std::shared_ptr<IShape>> m_shapes;
};