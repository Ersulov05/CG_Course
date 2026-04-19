#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "./TankData.h"
#include "../../../common/Geometry.h"

using json = nlohmann::json;

class TankDataParser 
{
public:
    static std::unordered_map<TankType, TankDataMapItem> ParseTanksData(const json& data) 
    {
        std::unordered_map<TankType, TankDataMapItem> tankDataMap;

        for (auto& [key, value] : data.items()) {
            TankType type = ConvertStringToTankType(key);
            TankDataMapItem tankDataMapItem = ParseTankData(value);
            
            tankDataMap[type] = tankDataMapItem;
        }

        return tankDataMap;
    }

private:
    static TankDataMapItem ParseTankData(const json& value) 
    {
        Size3D size = ParseSize(value);
        Point3D cannonPos = ParseCannonPos(value);
        float maxSpeed = ParseMaxSpeed(value);
        std::vector<TankLevelData> tankLevelsData = ParseTankLevelsData(value);
        TankData tankData = {size, cannonPos, maxSpeed};
            
        return TankDataMapItem{tankData, tankLevelsData};
    }

    static Size3D ParseSize(const json& value)
    {
        if (!value.contains("size")) {
            throw std::invalid_argument("Missing 'size' field");
        }
        
        if (!value["size"].is_array()) {
            throw std::invalid_argument("'size' must be an array");
        }
        
        if (value["size"].size() != 3) {
            throw std::invalid_argument("'size' must have exactly 3 elements (width, height, depth)");
        }

        return Size3D{
            value["size"][0].get<float>(),
            value["size"][1].get<float>(),
            value["size"][2].get<float>()
        };
    }

    static float ParseMaxSpeed(const json& value)
    {
        if (!value.contains("maxSpeed")) {
            throw std::invalid_argument("Missing 'maxSpeed' field");
        }
        
        if (!value["maxSpeed"].is_number()) {
            throw std::invalid_argument("'maxSpeed' must be an nummber");
        }

        return value["maxSpeed"].get<float>();
    }

    static Point3D ParseCannonPos(const json& value)
    {
        if (!value.contains("cannonPos")) {
            throw std::invalid_argument("Missing 'cannonPos' field");
        }
        
        if (!value["cannonPos"].is_array()) {
            throw std::invalid_argument("'cannonPos' must be an array");
        }
        
        if (value["cannonPos"].size() != 3) {
            throw std::invalid_argument("'cannonPos' must have exactly 3 elements (x, y, z)");
        }

        return Point3D{
            value["cannonPos"][0].get<float>(),
            value["cannonPos"][1].get<float>(),
            value["cannonPos"][2].get<float>()
        };
    }

    static std::vector<TankLevelData> ParseTankLevelsData(const json& value)
    {
        if (!value.contains("levels")) {
            throw std::invalid_argument("Missing 'levels' field");
        }
        
        if (!value["levels"].is_array()) {
            throw std::invalid_argument("'levels' must be an array");
        }
        
        if (value["levels"].empty()) {
            throw std::invalid_argument("'levels' array cannot be empty");
        }

        std::vector<TankLevelData> tankLevels;

        for (auto& levelData : value["levels"]) {
            TankLevelData level = ParseTankLevelData(levelData);
            tankLevels.push_back(level);
        }

        return tankLevels;
    }

    static TankLevelData ParseTankLevelData(const json& value)
    {        
        if (!value.contains("health") || !value["health"].is_number()) {
            throw std::invalid_argument("Invalid parse health");
        }

        if (!value.contains("acceleration") || !value["acceleration"].is_number()) {
            throw std::invalid_argument("Invalid parse acceleration");
        }

        if (!value.contains("cannonLevel") || !value["cannonLevel"].is_number()) {
            throw std::invalid_argument("Invalid parse cannonLevel");
        }

        unsigned int health = value["health"].get<unsigned int>();
        float acceleration = value["acceleration"].get<float>();
        unsigned int cannonLevel = value["cannonLevel"].get<unsigned int>();

        return TankLevelData{health, acceleration, cannonLevel};
    }
};