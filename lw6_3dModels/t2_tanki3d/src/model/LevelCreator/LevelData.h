#pragma once
#include <vector>
#include <string>
#include "../Map/Map.h"

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

class LevelData {
public:
    

    inline static const std::vector<LevelMap> LEVEL_MAPS = 
    {
        LevelMap{
            TerrarianMap{
                "ddddddddddddddd",
                "dddiidddddddddd",
                "dddiidddddddddd",
                "dddiidddddddddd",
                "dddiidddwwddddd",
                "ddddiiddddddddd",
                "dddddiiiiiiiiid",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
                "ddddddddddddddd",
            },
            ObjectsMap{
                "sssssssssssssss",
                "s0000000000000s",
                "s00000bb000000s",
                "sbbbbbb000bbbbs",
                "s00b0000000b00s",
                "s00b0b00000000s",
                "s00b00bb000000s",
                "s0000000000000s",
                "s00b000bbb00bbs",
                "s0000000000000s",
                "sbbb0000000b00s",
                "s0000000000b00s",
                "s0bbbbBBBbbb00s",
                "s00000BHB00000s",
                "sssssssssssssss",
            },
        },        
    };
};
