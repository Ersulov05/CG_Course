#pragma once

#include <vector>
#include "../shape/IShape.h"
#include <memory>
#include <unordered_map>

class Picture
{
public:
    void AddShape(std::shared_ptr<IShape> shape)
    {
        if (!shape)
            return;

        auto shapeId = shape->GetId();

        if (m_shapeMap.find(shapeId) != m_shapeMap.end())
        {
            throw std::runtime_error("Shape with ID " + std::to_string(shapeId) + " already exists");
        }

        m_shapes.push_back(shape);
        m_shapeMap[shapeId] = shape;
    }

    void MoveShape(unsigned shapeId, float dx, float dy)
    {
        auto it = m_shapeMap.find(shapeId);
        if (it != m_shapeMap.end() && it->second)
        {
            it->second->Move(dx, dy);
        }
    }

    std::shared_ptr<const IShape> GetShapeById(unsigned shapeId) const
    {
        auto it = m_shapeMap.find(shapeId);
        return (it != m_shapeMap.end()) ? std::static_pointer_cast<const IShape>(it->second) : nullptr;
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

    // std::shared_ptr<IShape> GetMutableShapeById(unsigned shapeId) override
    // {
    //     auto it = m_shapeMap.find(shapeId);
    //     return (it != m_shapeMap.end()) ? it->second : nullptr;
    // }

    // std::vector<std::shared_ptr<IShape>> GetMutableShapes() override
    // {
    //     return m_shapes;
    // }

    unsigned getNextId()
    {
        return m_nextId++;
    }

private:
    std::vector<std::shared_ptr<IShape>> m_shapes;
    std::unordered_map<unsigned, std::shared_ptr<IShape>> m_shapeMap;
    unsigned m_nextId = 0;
};