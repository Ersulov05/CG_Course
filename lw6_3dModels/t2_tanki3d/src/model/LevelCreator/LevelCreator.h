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
        auto levelObjects = GetLevelObjects(levelMap.objectsMap);

        float width = GetLevelWidth(levelMap.terrarianMap);
        float height = GetLevelHeight(levelMap.terrarianMap);

        return Map(terrarians, levelObjects.walls, levelObjects.headquartersWalls, levelObjects.headquarters, width, height);
    } 
private:  
    using TerrarianMap = std::vector<std::string>;
    using ObjectsMap = std::vector<std::string>;

    struct LevelMap {
        TerrarianMap terrarianMap;
        ObjectsMap objectsMap;
    };

    struct LevelObjects {
        Headquarters headquarters;
        std::vector<Wall> walls;
        std::vector<Wall> headquartersWalls;
    };
    
    inline static const std::vector<LevelMap> m_levelMaps = 
    {
        LevelMap{
            TerrarianMap{
                "dddddddddddd",
                "diiiiiiddddd",
                "diiiiiiddddd",
                "diiiiiiddddd",
                "diiiiiiddddd",
                "dddddddddddd",
                "dddddddddddd",
                "dddddddddddd",
                "dddddddddddd",
                "dddddddddddd",
                "dddddddddddd",
                "dddddddddddd",
            },
            ObjectsMap{
                "bbbbbbbbbbbb",
                "b0000000000b",
                "b0000000000b",
                "b0000000000b",
                "b0000000000b",
                "b0000000000b",
                "b00b0000000b",
                "b0000000000b",
                "b0000000000b",
                "b0000BBB000b",
                "b0000BHB000b",
                "bbbbbbbbbbbb",
            },
        },        
    };

    static float GetLevelWidth(const TerrarianMap& terrarianMap)
    {
        return (int)terrarianMap[0].size() * Constants::TERRARIAN_SIZE.width;
    }

    static float GetLevelHeight(const TerrarianMap& terrarianMap)
    {
        return (int)terrarianMap.size() * Constants::TERRARIAN_SIZE.depth;
    }

    static LevelMap GetLevelMap(unsigned int level)
    {
        return m_levelMaps[(level - 1) % m_levelMaps.size()];
    }

    static std::vector<Terrarian> GetTerrarians(const TerrarianMap& terrarianMap)
    {
        std::vector<Terrarian> terrarians;
        float xStart = -GetLevelWidth(terrarianMap) / 2;
        float x = xStart;
        float z = -GetLevelHeight(terrarianMap) / 2;

        for (const auto& row : terrarianMap) {
            for (const char& col : row) {
                auto terrarianType = ConvertCharToTerrarianType(col);
                Point3D terrarianPosition = {x, -Constants::TERRARIAN_SIZE.height/2, z};
                auto terrarian = Terrarian(terrarianType, terrarianPosition);
                terrarians.push_back(terrarian);

                x += Constants::TERRARIAN_SIZE.width;
            }
            z += Constants::TERRARIAN_SIZE.depth;
            x = xStart;
        }

        return terrarians;
    }

    static LevelObjects GetLevelObjects(const ObjectsMap& objectsMap)
    {
        LevelObjects levelObjects;
        float xStart = -(int)objectsMap[0].size() / 2 * Constants::DEFAULT_WALL_SIZE.width;
        float x = xStart;
        float z = -(int)objectsMap.size() / 2 * Constants::DEFAULT_WALL_SIZE.depth;

        for (const auto& row : objectsMap) {
            for (const char& obj : row) {
                HandleObjectItem(levelObjects, obj, x, z);
                x += Constants::DEFAULT_WALL_SIZE.width;
            }
            z += Constants::DEFAULT_WALL_SIZE.depth;
            x = xStart;
        }

        return levelObjects;
    }

    static void HandleObjectItem(LevelObjects& levelObjects, char obj, float x, float z)
    {
        if (obj == 'H') {
            Point3D headquartersPosition = {x, Constants::HEADQUARTERS_SIZE.height/2, z};
            levelObjects.headquarters = Headquarters(headquartersPosition);
            return;
        }

        auto wallType = ConvertCharToWallType(obj);
        if (!wallType.has_value()) 
        {
            return;
        }

        Point3D wallPosition = {x, Constants::DEFAULT_WALL_SIZE.height/2, z};
        auto wall = Wall(*wallType, wallPosition);
        if (obj == 'B') {
            levelObjects.headquartersWalls.push_back(wall);
            return;
        }
        levelObjects.walls.push_back(wall);
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
            case 'B':
                return WallType::Brick;
            case 's':
                return WallType::Steel;
            default:
                return std::nullopt;
        }
    }
};