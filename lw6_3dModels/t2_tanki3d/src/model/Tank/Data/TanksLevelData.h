#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

#include "./TankDataParser.h"
#include "./TankData.h"
#include "../../../lib/JsonLoader.h"

class TanksLevelData
{
public:
    static TankData GetTankDataByTankType(TankType type) {
        return GetTankDataItemByTankType(type).tankData;
    };

    static TankLevelData GetTankLevelData(const TankType& type, unsigned int level)
    {
        auto tankLevelsData = GetTankDataItemByTankType(type).tankLevelData;

        level = std::min((int)level, (int)tankLevelsData.size());

        return tankLevelsData[level - 1];
    }

private:
    static TankDataMapItem GetTankDataItemByTankType(TankType type) {
        auto it = m_tankDataMap.find(type);
        
        if (it != m_tankDataMap.end()) {
            return it->second;
        }

        throw std::invalid_argument("Unknown tankType: " + (int)type);
    };

    inline static std::unordered_map<TankType, TankDataMapItem> m_tankDataMap = 
        TankDataParser::ParseTanksData(JsonLoader::LoadFromFile("./data/tank_levels_data.json"));
};