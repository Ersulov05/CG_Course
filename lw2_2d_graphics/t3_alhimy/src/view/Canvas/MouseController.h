#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "../Common/Point.h"

class MouseController
{
public:
    using OnClickCallback = std::function<void(const Point &)>;
    using OnPressCallback = std::function<void(const Point &)>;
    using OnReleaseCallback = std::function<void(const Point &)>;
    using OnMoveCallback = std::function<void(const Point &)>;

    template <typename CallbackType>
    struct PrioritizedCallback
    {
        CallbackType callback;
        std::function<unsigned()> getPriority;
        unsigned id;

        unsigned GetCurrentPriority() const
        {
            return getPriority ? getPriority() : 0;
        }
    };

    void ProcessEvents(sf::Event &event)
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            HandleMousePress(event.mouseButton);
            break;

        case sf::Event::MouseButtonReleased:
            HandleMouseRelease(event.mouseButton);
            break;

        case sf::Event::MouseMoved:
            HandleMouseMove(event.mouseMove);
            break;

        case sf::Event::MouseLeft:
            HandleMouseLeft();
            break;

        default:
            break;
        }

        m_propagate = true;
    }

    unsigned OnMoveSubscribe(OnMoveCallback onMoveCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_moveCallbacks[id] = PrioritizedCallback{onMoveCallback, getZOrder, id};
        return id;
    }

    void OnMoveUnsubscribe(unsigned subscriptionId)
    {
        m_moveCallbacks.erase(subscriptionId);
    }

    unsigned OnReleaseSubscribe(OnReleaseCallback onReleaseCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_releaseCallbacks[id] = PrioritizedCallback{onReleaseCallback, getZOrder, id};
        return id;
    }

    void OnReleaseUnsubscribe(unsigned subscriptionId)
    {
        m_releaseCallbacks.erase(subscriptionId);
    }

    unsigned OnPressSubscribe(OnPressCallback onPressCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_pressCallbacks[id] = PrioritizedCallback{onPressCallback, getZOrder, id};
        return id;
    }

    void OnPressUnsubscribe(unsigned subscriptionId)
    {
        m_pressCallbacks.erase(subscriptionId);
    }

    unsigned OnClickSubscribe(OnClickCallback onClickCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_clickCallbacks[id] = PrioritizedCallback{onClickCallback, getZOrder, id};
        return id;
    }

    void OnClickUnsubscribe(unsigned subscriptionId)
    {
        m_clickCallbacks.erase(subscriptionId);
    }

    void ClearCallbacks()
    {
        m_clickCallbacks.clear();
        m_pressCallbacks.clear();
        m_releaseCallbacks.clear();
        m_moveCallbacks.clear();
    }

    void StopPropagation()
    {
        m_propagate = false;
    }

    Point GetMousePosition()
    {
        return m_mousePosition;
    }

private:
    void HandleMousePress(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left && !m_isPressed)
        {
            m_isPressed = true;
            NotifyCallbacks(m_pressCallbacks, Point(mouseButton.x, mouseButton.y));
        }
    }

    void HandleMouseRelease(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left && m_isPressed)
        {
            Point releasePos(mouseButton.x, mouseButton.y);
            NotifyCallbacks(m_releaseCallbacks, releasePos);
            NotifyCallbacks(m_clickCallbacks, releasePos);
            m_isPressed = false;
        }
    }

    void HandleMouseMove(const sf::Event::MouseMoveEvent &mouseMove)
    {
        m_mousePosition = Point(mouseMove.x, mouseMove.y);
        NotifyCallbacks(m_moveCallbacks, m_mousePosition);
    }

    void HandleMouseLeft()
    {
        if (m_isPressed)
        {
            NotifyCallbacks(m_releaseCallbacks, m_mousePosition);
        }
        m_isPressed = false;
    }

    template <typename MapType>
    void NotifyCallbacks(MapType &callbacks, const Point &position)
    {
        std::vector<typename MapType::mapped_type> copiedCallbacks;
        copiedCallbacks.reserve(callbacks.size());

        for (auto &[id, cb] : callbacks)
        {
            copiedCallbacks.push_back(cb);
        }

        std::sort(copiedCallbacks.begin(), copiedCallbacks.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a.GetCurrentPriority() > b.GetCurrentPriority();
                  });

        for (const auto &callback : copiedCallbacks)
        {
            if (!m_propagate)
                break;

            if (callback.callback)
            {
                callback.callback(position);
            }
        }
    }

    bool m_isPressed = false;
    bool m_propagate = true;
    Point m_mousePosition;

    std::unordered_map<unsigned, PrioritizedCallback<OnClickCallback>> m_clickCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnPressCallback>> m_pressCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnReleaseCallback>> m_releaseCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnMoveCallback>> m_moveCallbacks;

    // std::unordered_map<unsigned, OnClickCallback> m_clickCallbacks;
    // std::unordered_map<unsigned, OnPressCallback> m_pressCallbacks;
    // std::unordered_map<unsigned, OnReleaseCallback> m_releaseCallbacks;
    // std::unordered_map<unsigned, OnMoveCallback> m_moveCallbacks;
};