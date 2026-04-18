#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "../Cannon.h"
#include "./CannonData.h"

using json = nlohmann::json;

struct CannonLevelData {
    CannonType cannonType;
    MountType mountType;
    float reloadTime;
    unsigned int damage;
};

class CannonDataParser 
{
public:
    static std::vector<CannonLevelData> ParseCannonLevelsData(const json& data) 
    {        
        if (!data.contains("levels")) {
            throw std::invalid_argument("Missing 'levels' field");
        }
        
        if (!data["levels"].is_array()) {
            throw std::invalid_argument("'levels' must be an array");
        }
        
        if (data["levels"].empty()) {
            throw std::invalid_argument("'levels' array cannot be empty");
        }

        std::vector<CannonLevelData> cannonLevelsData;

        for (auto& levelData : data["levels"]) {
            CannonLevelData cannonLevelData = ParseCannonLevelData(levelData);

            cannonLevelsData.push_back(cannonLevelData);
        }

        return cannonLevelsData;
    }

private:
    static CannonLevelData ParseCannonLevelData(const json& data) 
    {
        if (!data.is_object()) {
            throw std::invalid_argument("Level data must be a JSON object");
        }

        CannonType cannonType = ParseCannonType(data);
        MountType mountType = ParseMountType(data);
        float reloadTime = ParseReloadTime(data);
        unsigned int damage = ParseDamage(data);
            
        return CannonLevelData{cannonType, mountType, reloadTime, damage};
    }

    static CannonType ParseCannonType(const json& data)
    {
        if (!data.contains("cannonType")) {
            throw std::invalid_argument("Missing 'cannonType' field");
        }

        return ConvertStringToCannonType(data["cannonType"]);
    }

    static MountType ParseMountType(const json& data)
    {
        if (!data.contains("mountType")) {
            throw std::invalid_argument("Missing 'mountType' field");
        }

        return ConvertStringToMountType(data["mountType"]);
    }

    static unsigned int ParseDamage(const json& data)
    {
        if (!data.contains("damage")) {
            throw std::invalid_argument("Missing 'damage' field");
        }

        if (!data["damage"].is_number()) {
            throw std::invalid_argument("'damage' must be a number");
        }

        return data["damage"].get<unsigned int>();
    }

    static float ParseReloadTime(const json& data)
    {
        if (!data.contains("reloadTime")) {
            throw std::invalid_argument("Missing 'reloadTime' field");
        }

        if (!data["reloadTime"].is_number()) {
            throw std::invalid_argument("'reloadTime' must be a number");
        }

        return data["reloadTime"].get<float>();
    }
};