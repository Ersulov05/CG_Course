#pragma once
#include "../common/Point3D.h"
#include "../common/Size3D.h"
#include "../common/Color.h"
#include "./Collision/BoxCollision.h"
#include <vector>

class Wall
{
public:
    Wall(Point3D position, Size3D size, Color color, std::vector<std::string> textures = {}): 
        m_position(position), 
        m_size(size), 
        m_color(color), 
        m_textures(textures),
        m_collision(BoxCollision(position, size, {0, 0, 0})) {}

    Point3D GetPosition() const {
        return m_position;
    }

    Size3D GetSize() const {
        return m_size;
    }

    Color GetColor() const {
        return m_color;
    }

    std::vector<std::string> GetTextures() const {
        return m_textures;
    }

    BoxCollision GetCollision() const {
        return m_collision;
    }
private:
    Point3D m_position;
    Size3D m_size;
    Color m_color;
    std::vector<std::string> m_textures;

    BoxCollision m_collision;
};