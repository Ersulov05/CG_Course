#pragma once
#include "./Data/CannonDataParser.h"
#include "../../../lib/JsonLoader.h"
#include "./Cannon.h"

class Cannon;
class CannonFactory {
public:
    static Cannon CreateCannonByLevel(unsigned int level)
    {
        CannonLevelData cannonData = GetCannonDataByLevel(level);

        return Cannon(
            cannonData.cannonType, 
            cannonData.mountType, 
            cannonData.reloadTime, 
            cannonData.damage
        );
    }
private:
    inline static const std::vector<CannonLevelData> CANNON_LEVELS_DATA = 
        CannonDataParser::ParseCannonLevelsData(JsonLoader::LoadFromFile("./data/cannon_levels_data.json"));

    static CannonLevelData GetCannonDataByLevel(unsigned int level)
    {
        level = std::min((int)level, (int)CANNON_LEVELS_DATA.size());

        return CANNON_LEVELS_DATA[level - 1];
    }
};