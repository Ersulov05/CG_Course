#pragma once
#include "./Wall/Wall.h"
#include "./Terrarian/Terrarian.h"
#include <vector>
#include <optional>
#include <iostream>

class Map {
public:
    Map() {}

    Map(const std::vector<Terrarian>& terrarians, const std::vector<Wall>& walls)
        : m_terrarians(terrarians),
        m_walls(walls) {}

    void Update() 
    {
        m_walls.erase(
            std::remove_if(
                m_walls.begin(), m_walls.end(),
                [](const Wall& wall) { return !wall.GetHealth() > 0; }
            ),
            m_walls.end()
        );
    }

    const std::vector<Terrarian>& GetTerrarians() const
    {
        return m_terrarians;
    }

    const std::vector<Wall>& GetWalls() const
    {
        return m_walls;
    }

    std::vector<Wall>& GetWalls() 
    {
        return m_walls;
    }

    std::optional<Terrarian> GetTerrarianByPosition(const Point3D position) const
    {
        for (auto& terrarian : m_terrarians) {
            Size3D size = terrarian.GetSize();
        
            float minX = terrarian.GetPosition().x - size.width / 2;
            float maxX = terrarian.GetPosition().x + size.width / 2;
            float minZ = terrarian.GetPosition().z - size.depth / 2;
            float maxZ = terrarian.GetPosition().z + size.depth / 2;
            
            if (position.x >= minX && position.x <= maxX &&
                position.z >= minZ && position.z <= maxZ) {
                return terrarian;
            }
        }

        return std::nullopt;
    }

private:
    std::vector<Terrarian> m_terrarians;
    std::vector<Wall> m_walls;
};