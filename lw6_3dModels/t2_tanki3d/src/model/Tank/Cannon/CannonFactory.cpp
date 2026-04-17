#include "./CannonFactory.h"
#include "./Cannon.h"
#include <stdexcept>
#include <iostream>

Cannon CannonFactory::CreateCannonByLevel(unsigned int level) {
    unsigned int damage;
    float reloadTime;
    switch (level) {
        case 1:
            damage = 50;
            reloadTime = 3;
            return Cannon(CannonType::MM85, MountType::SINGLE, reloadTime, damage);
        case 2:
            damage = 50;
            reloadTime = 4;
            return Cannon(CannonType::MM85, MountType::TWIN, reloadTime, damage);
        case 3:
            damage = 150;
            reloadTime = 5;
            return Cannon(CannonType::MM152, MountType::SINGLE, reloadTime, damage);
        case 4:
            damage = 150;
            reloadTime = 6;
            return Cannon(CannonType::MM152, MountType::TWIN, reloadTime, damage);
        default:
            throw std::invalid_argument("Unknown level: " + std::to_string(level));
    }
}