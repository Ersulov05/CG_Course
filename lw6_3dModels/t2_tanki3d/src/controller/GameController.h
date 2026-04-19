#pragma once
#include "../model/Game.h"
#include "../model/Tank/Tank.h"

class GameController {
public:
    GameController(Game& game): m_game(game) {}

    void Update(float deltatime)
    {
        m_game.Update(deltatime);
    }

    void RotateTank(Direction rotateDirection)
    {
        auto tank = m_game.GetPlayerTank();
        tank->Rotate(rotateDirection);
    }

    void MoveTank()
    {
        auto tank = m_game.GetPlayerTank();
        tank->Move();
    }

    void StopTank()
    {
        auto tank = m_game.GetPlayerTank();
        tank->Stop();
    }

    void Fire() 
    {
        auto tank = m_game.GetPlayerTank();
        auto newShells = tank->Fire();

        auto& shells = m_game.GetShellManager().GetShells();
        shells.insert(shells.end(), newShells.begin(), newShells.end());
    }

    const Map& GetMap() const 
    {
        return m_game.GetMap();
    }

    const std::vector<Bonus>& GetBonuses() const
    {
        return m_game.GetBonusManager().GetBonuses();
    }

    const std::vector<Shell>& GetShells() const 
    {
        return m_game.GetShellManager().GetShells();
    }

    const std::shared_ptr<Tank> GetPlayerTank() const 
    {
        return m_game.GetPlayerTank();
    }

    const std::vector<std::shared_ptr<Tank>>& GetEnemyTanks() const
    {
        return m_game.GetEnemyTanks();
    }

    const std::vector<std::shared_ptr<IEffect>>& GetEffects() const
    {
        return m_game.GetEffectManager().GetEffects();
    }

private:
    Game& m_game;
};