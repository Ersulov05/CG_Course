#pragma once
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"
#include "../Constants.h"
#include <optional>

#include "../Tank/Tank.h"
#include "../Map/Wall/Wall.h"
#include "./CollisionDetector.h"

class CollisionHandler
{
public:
    static bool CheckAndHandleCollision(Shell& shell, std::shared_ptr<Tank> tank)
    {
        if (CollisionDetector::Detect(shell, tank))
        {
            tank->TakeDamage(shell.GetDamage());
            shell.Boom();
            return true;
        }

        return false;
    }

    static bool CheckAndHandleCollision(Shell& shell, Map& map)
    {
        if (CheckAndHandleCollision(shell, map.GetHeadquarters()))
        {
            return true;
        }
        for (auto& wall : map.GetWalls()) 
        {
            if (CheckAndHandleCollision(shell, wall)) {
                return true;
            }
        }
        for (auto& wall : map.GetHeadquartersWalls()) 
        {
            if (CheckAndHandleCollision(shell, wall)) {
                return true;
            }
        }

        return false;
    }

    static void CheckAndHandleCollision(std::shared_ptr<Tank> tank, Map& map)
    {
        CheckAndHandleCollision(tank, map.GetHeadquarters());
        for (auto& wall : map.GetWalls()) {
            CheckAndHandleCollision(tank, wall);
        }
        for (auto& wall : map.GetHeadquartersWalls()) 
        {
            CheckAndHandleCollision(tank, wall);
        }
    }

    static void CheckAndHandleCollision(std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2)
    {
        auto collisionData = CollisionDetector::Detect(tank1, tank2);
        if (!collisionData.has_value()) {
            return;
        }

        auto firstSpeed = tank1->GetSpeed().GetLength();
        auto secondSpeed = tank2->GetSpeed().GetLength();
        auto summ = firstSpeed + secondSpeed;
        auto firstCoef = summ >= 0 ? firstSpeed / summ : 0.5;
        auto secondCoef = summ >= 0 ? secondSpeed / summ : 0.5;
        CollisionData firstCollisionData = *collisionData;
        CollisionData secondCollisionData = *collisionData;
        secondCollisionData.normal = secondCollisionData.normal * -1;
        secondCollisionData.overlap *= secondCoef;
        firstCollisionData.overlap *= firstCoef;

        CorrectTankBySpeedAndNormal(tank1, firstCollisionData);
        CorrectTankBySpeedAndNormal(tank2, secondCollisionData);

        collisionData = CollisionDetector::Detect(tank1, tank2);
        if (!collisionData.has_value()) {
            return;
        }

        firstCollisionData, secondCollisionData = *collisionData;
        secondCollisionData.normal = secondCollisionData.normal * -1;
        secondCollisionData.overlap *= secondCoef;
        firstCollisionData.overlap *= firstCoef;

        CorrectTankByNormal(tank1, firstCollisionData);
        CorrectTankByNormal(tank2, secondCollisionData);
    }

private:
    static bool CheckAndHandleCollision(Shell& shell, Wall& wall)
    {
        if (CollisionDetector::Detect(shell, wall)) {
            wall.TakeDamage();
            shell.Boom();
            return true;
        }
        return false;
    }

    static void CheckAndHandleCollision(std::shared_ptr<Tank> tank, const Wall &wall)
    {
        auto collisionData = CollisionDetector::Detect(tank, wall);
        if (!collisionData.has_value()) {
            return;
        }

        CorrectTankBySpeedAndNormal(tank, *collisionData);

        collisionData = CollisionDetector::Detect(tank, wall);
        if (!collisionData.has_value()) {
            return;
        }

        CorrectTankByNormal(tank, *collisionData);
    }

    static void CheckAndHandleCollision(std::shared_ptr<Tank> tank, const Headquarters &headquarters)
    {
        auto collisionData = CollisionDetector::Detect(tank, headquarters);
        if (!collisionData.has_value()) {
            return;
        }

        CorrectTankBySpeedAndNormal(tank, *collisionData);

        collisionData = CollisionDetector::Detect(tank, headquarters);
        if (!collisionData.has_value()) {
            return;
        }

        CorrectTankByNormal(tank, *collisionData);
    }

    static bool CheckAndHandleCollision(Shell& shell, Headquarters& headquarters)
    {
        if (CollisionDetector::Detect(shell, headquarters))
        {
            headquarters.TakeDamage(shell.GetDamage());
            shell.Boom();
            return true;
        }
        return false;
    }

    static void CorrectTankBySpeedAndNormal(std::shared_ptr<Tank> tank, const CollisionData& collisionData)
    {
        Vector3D speed = tank->GetSpeed();
        Vector3D normal = collisionData.normal;
        float overlap = collisionData.overlap + 0.01;
        
        Vector3D correction(0, 0, 0);
        
        if (normal.x != 0) {
            if (speed.x * normal.x > 0) 
            {
                correction.x = normal.x * overlap;
            }
        }
        
        if (normal.z != 0) {
            if (speed.z * normal.z > 0) {
                correction.z = normal.z * overlap;
            }
        }
        
        Point3D newPosition = tank->GetPosition();
        newPosition.x += correction.x;
        newPosition.z += correction.z;
        tank->SetPosition(newPosition);
        
        Vector3D newSpeed = speed;
        if (correction.x != 0) {
            newSpeed.x = 0;
        }
        if (correction.z != 0) {
            newSpeed.z = 0;
        }
        tank->SetSpeed(newSpeed);
    }

    static void CorrectTankByNormal(std::shared_ptr<Tank> tank, const CollisionData& collisionData)
    {
        Vector3D speed = tank->GetSpeed();
        Vector3D normal = collisionData.normal;
        float overlap = collisionData.overlap + 0.01;
        
        Vector3D correction = normal * overlap;
        
        Point3D newPosition = tank->GetPosition();
        newPosition.x += correction.x;
        newPosition.z += correction.z;
        tank->SetPosition(newPosition);
    }
};