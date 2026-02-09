#pragma once

#include "../picture/Picture.h"
#include "../Observable.h"
#include "../shape/Rectangle.h"
#include <optional>

class Scene
{
public:
    void AddPicture(std::shared_ptr<Picture> picture)
    {
        if (!picture)
            return;

        auto pictureId = picture->GetId();

        if (m_pictureMap.find(pictureId) != m_pictureMap.end())
        {
            throw std::runtime_error("Shape with ID " + std::to_string(pictureId) + " already exists");
        }

        m_pictures.push_back(picture);
        m_pictureMap[pictureId] = picture;
    }

    void AddShapeToPicture(unsigned pictureId, std::shared_ptr<IShape> shape)
    {
        auto picture = GetPictureById(pictureId);
        picture->AddShape(shape);
    }

    // void AddRectangleToPicture(unsigned pictureId, Point position, float width, float height, Color fillColor = 0xFFFFFFFF, Color outlineColor = 0xFFFFFFFF)
    // {
    //     auto picture = GetPictureById(pictureId);
    //     auto shape = std::make_shared<Rectangle>(position, width, height, fillColor, outlineColor);
    //     picture->AddShape(shape);
    // }

    // void AddTriangleToPicture(unsigned pictureId, Point firstPoint, Point secondPoint, Point thirdPoint, Color fillColor = 0xFFFFFFFF, Color outlineColor = 0x0)
    // {
    //     auto shape = std::make_shared<Triangle>(position, width, height, fillColor, outlineColor);
    //     m_scene.AddRectangleToPicture(pictureId, firstPoint, secondPoint, thirdPoint, fillColor, outlineColor);
    // }

    void MovePicture(unsigned pictureId, float dx, float dy)
    {
        auto it = m_pictureMap.find(pictureId);
        if (it != m_pictureMap.end() && it->second)
        {
            it->second->Move(dx, dy);
        }
    }

    std::shared_ptr<const Picture> GetPictureById(unsigned pictureId) const
    {
        auto it = m_pictureMap.find(pictureId);
        return (it != m_pictureMap.end()) ? std::static_pointer_cast<const Picture>(it->second) : nullptr;
    }

    std::shared_ptr<Picture> GetPictureById(unsigned shapeId)
    {
        auto it = m_pictureMap.find(shapeId);
        return (it != m_pictureMap.end()) ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<const Picture>> GetPictures() const
    {
        std::vector<std::shared_ptr<const Picture>> result;
        result.reserve(m_pictures.size());

        for (const auto &picture : m_pictures)
        {
            result.push_back(std::static_pointer_cast<const Picture>(picture));
        }

        return result;
    }

    size_t GetPictureCount() const
    {
        return m_pictures.size();
    }

    unsigned getNextId()
    {
        return m_nextId++;
    }

    std::optional<unsigned> GetPictureIdByPoint(const Point &point) const
    {
        for (const auto &picture : m_pictures)
        {
            if (picture->IsPointInsidePicture(point))
            {
                return picture->GetId();
            }
        }
        return std::nullopt;
    }

private:
    std::vector<std::shared_ptr<Picture>> m_pictures;
    std::unordered_map<unsigned, std::shared_ptr<Picture>> m_pictureMap;
    unsigned m_nextId = 1;
};