#pragma once
#include "./Wall/Wall.h"
#include "./Terrarian/Terrarian.h"
#include "./Headquarters/Headquarters.h"
#include <vector>
#include <optional>
#include <iostream>

class Map {
public:
    Map() {}
    Map(
        const std::vector<Terrarian>& terrarians, 
        const std::vector<Wall>& walls,
        const std::vector<Wall>& headquartersWalls,
        const Headquarters& headquarters,
        float width,
        float height
    ) 
        : m_terrarians(terrarians)
        , m_walls(walls)
        , m_originalHeadquartersWalls(headquartersWalls)
        , m_headquartersWalls(headquartersWalls)
        , m_headquarters(headquarters)
        , m_width(width)
        , m_height(height) 
    {
    }

    void Update() 
    {
        std::erase_if(m_walls, [](const Wall& wall) { 
            return !wall.GetHealth() > 0; 
        });

        std::erase_if(m_headquartersWalls, [](const Wall& wall) { 
            return !wall.GetHealth() > 0; 
        });
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

    const std::vector<Wall>& GetHeadquartersWalls() const
    {
        return m_headquartersWalls;
    }

    std::vector<Wall>& GetHeadquartersWalls() 
    {
        return m_headquartersWalls;
    }

    const Headquarters& GetHeadquarters() const
    {
        return m_headquarters;
    }

    Headquarters& GetHeadquarters()
    {
        return m_headquarters;
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

    float GetWidth() const
    {
        return m_width;
    }

    float GetHeight() const
    {
        return m_height;
    }

    void RepairHeadquartersWalls()
    {
        m_headquartersWalls.clear();
        m_headquartersWalls = m_originalHeadquartersWalls;
    }

private:
    Headquarters m_headquarters;
    std::vector<Terrarian> m_terrarians;
    std::vector<Wall> m_walls;
    std::vector<Wall> m_headquartersWalls;
    std::vector<Wall> m_originalHeadquartersWalls;
    float m_width;
    float m_height;
};