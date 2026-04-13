#pragma once
#include "./Car.h"

class Game {
public:
    void Update(float deltatime) {
        m_policeCar.Update(deltatime);
        m_car.Update(deltatime);
    }

    Car GetPoliceCar() const {
        return m_policeCar;
    }

    Car GetCar() const {
        return m_car;
    }
private:
    Car m_policeCar = Car({1, 0, 9});
    Car m_car = Car({1, 0, 4});
};