#pragma once
#include "../../../common/Geometry.h"
#include "../../Shell/Shell.h"
#include "./Data/CannonData.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>

class Cannon {
public:
    Cannon(CannonType cannonType, MountType mountType, float totalReloadTime, unsigned int damage);

    void Update(float deltatime);

    std::vector<Shell> Fire(const Point3D tankPosition, const Quaternion3D tankRotation);

    float GetReloadTime() const;

    float GetTotalReloadTime() const;

    CannonType GetCannonType() const;

    MountType GetMountType() const;

    float GetCannonOffset() const;

    Point3D GetLocalPosition() const;

    void SetLocalPosition(const Point3D& position);

private:
    struct ShellData {
        ShellType type;
        Size3D size;
        float speed;
    };

    float m_reloadTime = 0;
    float m_totalReloadTime;
    unsigned int m_damage;
    float m_shellSpeed = 10;
    CannonType m_cannonType;
    MountType m_mountType;
    Point3D m_localPosition;

    ShellData GetShellData() const;
};