#pragma once
#include "../Map/Map.h"
#include <string>
#include <vector>
#include "../Constants.h"
#include <optional>

class LevelCreator {
public:
    static Map GetMapByLevel(unsigned int level) {
 
        auto levelMap = GetLevelMap(level);

        auto terrarians = GetTerrarians(levelMap.terrarianMap);
        auto walls = GetWalls(levelMap.wallMap);

        return Map(terrarians, walls);
    } 
private:  
    using TerrarianMap = std::vector<std::string>;
    using WallMap = std::vector<std::string>;

    struct LevelMap {
        TerrarianMap terrarianMap;
        WallMap wallMap;
    };
    // using LevelMap = std::pair<TerrarianMap, WallMap>;
    
    inline static const std::vector<LevelMap> m_levelMaps = 
    {
        LevelMap{
            TerrarianMap{
                "dddddddd",
                "diiiiiid",
                "diiiiiid",
                "diiiiiid",
                "diiiiiid",
                "dddddddd",
                "dddddddd",
                "dddddddd",
            },
            WallMap{
                "bbbbbbbb",
                "b000000b",
                "b00b000b",
                "b000000b",
                "b000000b",
                "b000000b",
                "b000000b",
                "bbbbbbbb",
            },
        },        
    };

    static LevelMap GetLevelMap(unsigned int level)
    {
        return m_levelMaps[(level - 1) % m_levelMaps.size()];
    }

    static std::vector<Terrarian> GetTerrarians(TerrarianMap terrarianMap)
    {
        std::vector<Terrarian> terrarians;
        float xStart = -(int)terrarianMap[0].size() / 2 * Constants::TERRARIAN_SIZE.width;
        float x = xStart;
        float z = -(int)terrarianMap.size() / 2 * Constants::TERRARIAN_SIZE.depth;

        for (auto& row : terrarianMap) {
            for (char& col : row) {
                auto terrarianType = ConvertCharToTerrarianType(col);
                Point3D terrarianPosition = {x, 0, z};
                auto terrarian = Terrarian(terrarianType, {x, 0, z});
                terrarians.push_back(terrarian);

                x += Constants::TERRARIAN_SIZE.width;
            }
            z += Constants::TERRARIAN_SIZE.depth;
            x = xStart;
        }

        return terrarians;
    }

    static std::vector<Wall> GetWalls(WallMap wallMap)
    {
        std::vector<Wall> walls;
        float xStart = -(int)wallMap[0].size() / 2 * Constants::DEFAULT_WALL_SIZE.width;
        float x = xStart;
        float z = -(int)wallMap.size() / 2 * Constants::DEFAULT_WALL_SIZE.depth;

        for (auto& row : wallMap) {
            for (char& col : row) {
                auto wallType = ConvertCharToWallType(col);
                if (!wallType.has_value()) 
                {
                    x += Constants::DEFAULT_WALL_SIZE.width;
                    continue;
                }
                Point3D wallPosition = {x, Constants::DEFAULT_WALL_SIZE.height/2, z};
                auto wall = Wall(*wallType, wallPosition);
                walls.push_back(wall);

                x += Constants::DEFAULT_WALL_SIZE.width;
            }
            z += Constants::DEFAULT_WALL_SIZE.depth;
            x = xStart;
        }

        return walls;
    }

    static TerrarianType ConvertCharToTerrarianType(char ch) 
    {
        switch (ch) {
            case 'd':
                return TerrarianType::Dirt;
            case 'i':
                return TerrarianType::Ice;
            default:
                return TerrarianType::Dirt;
        }
    }

    static std::optional<WallType> ConvertCharToWallType(char ch) 
    {
        switch (ch) {
            case 'b':
                return WallType::Brick;
            case 's':
                return WallType::Steel;
            default:
                return std::nullopt;
        }
    }
};