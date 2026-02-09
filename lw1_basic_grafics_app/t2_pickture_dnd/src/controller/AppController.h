#pragma once

#include "../model/scene/Scene.h"
#include "../model/picture/Picture.h"
#include "../model/shape/Rectangle.h"
#include "../model/shape/Triangle.h"
#include "../model/shape/IShape.h"
#include "../model/common/Point.h"
#include <iostream>
#include <memory>
#include <vector>
#include <optional>

class AppController
{
public:
    AppController(Scene &scene) : m_scene(scene) {}

    void AddRectangleToPicture(unsigned pictureId, float x, float y, float width, float height, Color fillColor = 0xFFFFFFFF, Color outlineColor = 0x0)
    {
        auto shape = std::make_shared<Rectangle>(Point(x, y), width, height, fillColor, outlineColor);
        m_scene.AddShapeToPicture(pictureId, shape);
    }

    void AddTriangleToPicture(unsigned pictureId, Point firstPoint, Point secondPoint, Point thirdPoint, Color fillColor = 0xFFFFFFFF, Color outlineColor = 0x0)
    {
        auto shape = std::make_shared<Triangle>(firstPoint, secondPoint, thirdPoint, fillColor, outlineColor);
        m_scene.AddShapeToPicture(pictureId, shape);
    }

    unsigned CreatePicture()
    {
        auto pictureId = m_scene.getNextId();
        auto picture = std::make_shared<Picture>(pictureId);
        m_scene.AddPicture(picture);
        return pictureId;
    }

    void MovePicture(unsigned pictureId, float dx, float dy)
    {
        m_scene.MovePicture(pictureId, dx, dy);
    }

    std::vector<std::shared_ptr<const Picture>> GetPictures() const
    {
        return m_scene.GetPictures();
    }

    std::optional<unsigned> GetPictureIdByPoint(const Point &point) const
    {
        return m_scene.GetPictureIdByPoint(point);
    }

    const Scene GetScene() const
    {
        return m_scene;
    }

private:
    Scene &m_scene;
};