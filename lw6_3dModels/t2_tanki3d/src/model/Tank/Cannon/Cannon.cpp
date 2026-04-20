#include "./Cannon.h"
#include "../../../common/Geometry.h"
#include "../../Shell/Shell.h"
#include <stdexcept>
#include <string>

Cannon::Cannon(CannonType cannonType, MountType mountType, float totalReloadTime, unsigned int damage)
    : m_cannonType(cannonType),
        m_mountType(mountType),
        m_totalReloadTime(totalReloadTime),
        m_damage(damage) 
{
}

void Cannon::Update(float deltatime) {
    if (m_reloadTime > 0) {
        m_reloadTime -= deltatime;
    }
}

std::vector<Shell> Cannon::Fire(const Point3D tankPosition, const Quaternion3D tankRotation) 
{
    if (m_reloadTime > 0) 
    {
        return {};
    }
    m_reloadTime = m_totalReloadTime;
    
    auto shellData = GetShellData();
    if (m_mountType == MountType::SINGLE) {
        Point3D shellPos = tankPosition + tankRotation * m_localPosition;
        return {Shell(shellPos, shellData.size, tankRotation, m_damage, shellData.speed)};
    } else {
        Point3D leftShellPos = tankPosition + tankRotation * m_localPosition + tankRotation.GetRight() * -GetCannonOffset();
        Point3D rightShellPos = tankPosition + tankRotation * m_localPosition + tankRotation.GetRight() * GetCannonOffset();
        return {
            Shell(leftShellPos, shellData.size, tankRotation, m_damage, shellData.speed),
            Shell(rightShellPos, shellData.size, tankRotation, m_damage, shellData.speed)
        };
    }
}

float Cannon::GetReloadTime() const 
{
    return m_reloadTime;
}

float Cannon::GetTotalReloadTime() const
{
    return m_totalReloadTime;
}

CannonType Cannon::GetCannonType() const 
{
    return m_cannonType;
}

MountType Cannon::GetMountType() const 
{
    return m_mountType;
}

Point3D Cannon::GetLocalPosition() const
{
    return m_localPosition;
}

void Cannon::SetLocalPosition(const Point3D& position)
{
    m_localPosition = position;
}

void Cannon::SetTotalReloadTime(float totalReloadTime)
{
    m_totalReloadTime = totalReloadTime;
    m_reloadTime = std::max(0.0f, std::min(m_reloadTime, m_totalReloadTime));
}

void Cannon::SetReloadTime(float reloadTime)
{
    m_reloadTime = std::max(0.0f, std::min(reloadTime, m_totalReloadTime));
}

Cannon::ShellData Cannon::GetShellData() const
{
    float speed;
    Size3D size;

    switch (m_cannonType) {
        case CannonType::MM152:
            speed = 20;
            size = {0.17, 0.17, 0.32};
            return ShellData{ShellType::OF, size, speed};
        case CannonType::MM85:
            speed = 30;
            size = {0.1, 0.1, 0.25};
            return ShellData{ShellType::BB, size, speed};
        case CannonType::MM100:
            speed = 25;
            size = {0.12, 0.12, 0.3};
            return ShellData{ShellType::BB, size, speed};
        default:
            throw std::invalid_argument("Unknown cannonType: "  + (int)m_cannonType);
    }
}

float Cannon::GetCannonOffset() const
{
    switch (m_cannonType) {
        case CannonType::MM85:
            return 0.15;
        case CannonType::MM152:
            return 0.2;
        default:
            return 0.15;
    }
}