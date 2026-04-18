#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>

enum class MountType {
    SINGLE = 1,
    TWIN = 2,
};

enum class CannonType {
    MM152,
    MM85
};

inline static std::unordered_map<std::string, CannonType> STRING_TO_CANNON_TYPE = 
{
    {"MM85", CannonType::MM85},
    {"MM152", CannonType::MM152},
};

inline static std::unordered_map<std::string, MountType> STRING_TO_MOUNT_TYPE = 
{
    {"SINGLE", MountType::SINGLE},
    {"TWIN", MountType::TWIN},
};

inline static CannonType ConvertStringToCannonType(const std::string& type)
{
    auto it = STRING_TO_CANNON_TYPE.find(type);
        
    if (it != STRING_TO_CANNON_TYPE.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown cannon type: " + type);
}

inline static MountType ConvertStringToMountType(const std::string& type)
{
    auto it = STRING_TO_MOUNT_TYPE.find(type);
        
    if (it != STRING_TO_MOUNT_TYPE.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown mount type: " + type);
}