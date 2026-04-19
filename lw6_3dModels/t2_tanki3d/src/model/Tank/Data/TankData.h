#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>
#include "../../../common/Geometry.h"

enum class TankType 
{
    T34,
    TIGER,
};

inline static std::unordered_map<std::string, TankType> STRING_TO_TANK_TYPE = 
{
    {"T34", TankType::T34},
    {"Tiger", TankType::TIGER},
};

struct TankLevelData {
    unsigned int health;
    float acceleration;
    unsigned int cannonLevel;
};

struct TankData {
    Size3D size;
    Point3D cannonPos;
    float maxSpeed;
};

struct TankDataMapItem {
    TankData tankData;
    std::vector<TankLevelData> tankLevelData;
};

inline static TankType ConvertStringToTankType(const std::string& type)
{
    auto it = STRING_TO_TANK_TYPE.find(type);
        
    if (it != STRING_TO_TANK_TYPE.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown tank type: " + type);
}