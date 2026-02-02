#pragma once

#include "../model/picture/Picture.h"
#include "../model/shape/Rectangle.h"
#include "../model/shape/IShape.h"
#include "../model/common/Point.h"
#include <iostream>
#include <memory>
#include <vector>

class AppController
{
public:
    AppController(Picture &picture) : m_picture(picture) {}

    void AddRectangle(float x, float y, float width, float height)
    {
        auto shapeId = m_picture.getNextId();
        auto shape = std::make_shared<Rectangle>(shapeId, Point(x, y), width, height, 0xFFFFFFFF, 0xFFFFFFFF);
        m_picture.AddShape(shape);
    }

    void MoveShape(unsigned shapeId, float dx, float dy)
    {
        m_picture.MoveShape(shapeId, dx, dy);
    }

    std::vector<std::shared_ptr<const IShape>> GetShapes()
    {
        return m_picture.GetShapes();
    }

private:
    Picture &m_picture;
};