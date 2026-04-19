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
        auto levelObjects = GetLevelObjects(levelMap);

        float width = GetLevelWidth(levelMap.terrarianMap);
        float height = GetLevelHeight(levelMap.terrarianMap);

        return Map(levelObjects.terrarians, levelObjects.walls, levelObjects.headquartersWalls, levelObjects.headquarters, width, height);
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
        std::vector<Terrarian> terrarians;
    };
    
    inline static const std::vector<LevelMap> m_levelMaps = 
    {
        LevelMap{
            TerrarianMap{
                "ddddddddddddd",
                "diiiiiidddddd",
                "diiiiiidddddd",
                "diiiiiidddddd",
                "diiiiiiddwddd",
                "ddddddddddddd",
                "ddddddddddddd",
                "ddddddddddddd",
                "ddddddddddddd",
                "ddddddddddddd",
                "ddddddddddddd",
                "ddddddddddddd",
            },
            ObjectsMap{
                "sssssssssssss",
                "s00000000000s",
                "s00000000000s",
                "s00000000000s",
                "s00000000000s",
                "s00000000000s",
                "s00b00000000s",
                "s00000000000s",
                "s00000000000s",
                "s0000BBB0000s",
                "s0000BHB0000s",
                "sssssssssssss",
            },
        },        
    };

    static float GetLevelWidth(const TerrarianMap& terrarianMap)
    {
        std::cout << (int)terrarianMap[0].size() * Constants::TERRARIAN_SIZE.width << std::endl;
        return (int)terrarianMap[0].size() * Constants::TERRARIAN_SIZE.width;
    }

    static float GetLevelHeight(const TerrarianMap& terrarianMap)
    {
        std::cout << (int)terrarianMap.size() * Constants::TERRARIAN_SIZE.depth << std::endl;
        return (int)terrarianMap.size() * Constants::TERRARIAN_SIZE.depth;
    }

    static LevelMap GetLevelMap(unsigned int level)
    {
        return m_levelMaps[(level - 1) % m_levelMaps.size()];
    }

    static LevelObjects GetLevelObjects(const LevelMap& levelMap)
    {
        LevelObjects levelObjects;
        SetLevelTerrarians(levelMap.terrarianMap, levelObjects);
        SetLevelObjects(levelMap.objectsMap, levelObjects);

        return levelObjects;
    }

    static void SetLevelTerrarians(const TerrarianMap& terrarianMap, LevelObjects& levelObjects)
    {
        float xStart = -GetLevelWidth(terrarianMap) / 2 + Constants::TERRARIAN_SIZE.width/2;
        float x = xStart;
        float z = -GetLevelHeight(terrarianMap) / 2 + Constants::TERRARIAN_SIZE.depth/2;

        for (const auto& row : terrarianMap) {
            for (const char& col : row) {
                HandleTerrarianItem(levelObjects, col, x, z);
                x += Constants::TERRARIAN_SIZE.width;
            }
            z += Constants::TERRARIAN_SIZE.depth;
            x = xStart;
        }
    }

    static void SetLevelObjects(const ObjectsMap& objectsMap, LevelObjects& levelObjects)
    {
        float xStart = -(float)objectsMap[0].size() / 2 * Constants::DEFAULT_WALL_SIZE.width + Constants::DEFAULT_WALL_SIZE.width/2;
        float x = xStart;
        float z = -(float)objectsMap.size() / 2 * Constants::DEFAULT_WALL_SIZE.depth + Constants::DEFAULT_WALL_SIZE.depth/2;

        for (const auto& row : objectsMap) {
            for (const char& obj : row) {
                HandleObjectItem(levelObjects, obj, x, z);
                x += Constants::DEFAULT_WALL_SIZE.width;
            }
            z += Constants::DEFAULT_WALL_SIZE.depth;
            x = xStart;
        }
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

    static void HandleTerrarianItem(LevelObjects& levelObjects, char terr, float x, float z)
    {
        auto terrarianType = ConvertCharToTerrarianType(terr);
        if (terrarianType == TerrarianType::Water) {
            Point3D wallPosition = {x, Constants::DEFAULT_WALL_SIZE.height/2, z};
            Size3D wallSize = Constants::TERRARIAN_SIZE;
            wallSize.height = Constants::DEFAULT_WALL_SIZE.height;
            auto wall = Wall(WallType::Empty, wallPosition, wallSize);
            levelObjects.walls.push_back(wall);
        }
        Point3D terrarianPosition = {x, -Constants::TERRARIAN_SIZE.height/2, z};
        auto terrarian = Terrarian(terrarianType, terrarianPosition);
        levelObjects.terrarians.push_back(terrarian);
    }

    static TerrarianType ConvertCharToTerrarianType(char ch) 
    {
        switch (ch) {
            case 'w':
                return TerrarianType::Water;
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