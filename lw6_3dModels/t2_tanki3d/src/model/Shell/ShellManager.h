#pragma once
#include <vector>
#include "./Shell.h"
#include <functional>

class ShellManager {
public:
    void Update(float deltatime) 
    {
        for (auto& shell : m_shells) {
            shell.Update(deltatime);
        }
        
        m_shells.erase(
            std::remove_if(
                m_shells.begin(), m_shells.end(),
                [](const Shell& shell) { return !shell.IsAlive(); }
            ),
            m_shells.end()
        );
    }

    void AddShell(const Shell& shell)
    {
        m_shells.push_back(shell);
    }

    void AddShells(const std::vector<Shell>& shells)
    {
        m_shells.insert(m_shells.end(), shells.begin(), shells.end());
    }

    std::vector<Shell>& GetShells()
    {
        return m_shells;
    }

    const std::vector<Shell>& GetShells() const
    {
        return m_shells;
    }

    void Clear()
    {
        m_shells.clear();
    }
private:
    std::vector<Shell> m_shells;
};