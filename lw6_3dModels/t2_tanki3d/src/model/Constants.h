#pragma once
#include "../common/Geometry.h"
class Constants {
public:
    inline static const Size3D TERRARIAN_SIZE = {6, 0.1, 6};
    inline static const Size3D HEADQUARTERS_SIZE = {5, 2, 5};
    inline static const unsigned int HEADQUARTERS_HEALTH = 1000;
    inline static const Size3D DEFAULT_WALL_SIZE = {6, 6, 6};
    inline static const Size3D BONUS_SIZE = {2, 2, 2};
    inline static const float BONUS_LIFETIME = 20;
    inline static const int MAX_BONUSES = 4;
    inline static const float BONUS_RESPAWN_TIME = 5;
    inline static const float ENEMY_RESPAWN_TIME = 3;
    inline static const int MAX_ENEMIES = 3;
    inline static const Point3D PLAYER_POS = {0, 0, 18};
};
