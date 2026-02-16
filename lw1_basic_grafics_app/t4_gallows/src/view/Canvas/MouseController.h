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
    }

    unsigned OnMoveSubscribe(OnMoveCallback onMoveCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_moveCallbacks[id] = onMoveCallback;
        return id;
    }

    void OnMoveUnsubscribe(unsigned subscriptionId)
    {
        m_moveCallbacks.erase(subscriptionId);
    }

    unsigned OnReleaseSubscribe(OnReleaseCallback onReleaseCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_releaseCallbacks[id] = onReleaseCallback;
        return id;
    }

    void OnReleaseUnsubscribe(unsigned subscriptionId)
    {
        m_releaseCallbacks.erase(subscriptionId);
    }

    unsigned OnPressSubscribe(OnPressCallback onPressCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_pressCallbacks[id] = onPressCallback;
        return id;
    }

    void OnPressUnsubscribe(unsigned subscriptionId)
    {
        m_pressCallbacks.erase(subscriptionId);
    }

    unsigned OnClickSubscribe(OnClickCallback onClickCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_clickCallbacks[id] = onClickCallback;
        return id;
    }

    void OnClickUnsubscribe(unsigned subscriptionId)
    {
        m_clickCallbacks.erase(subscriptionId);
    }

    void ClearCallbacks()
    {
        m_clickCallbacks.clear();
    }

private:
    void HandleMousePress(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left)
        {
            m_isPressed = true;
            NotifyPressCallbacks(Point(mouseButton.x, mouseButton.y))
        }
    }

    void HandleMouseRelease(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left && m_isPressed)
        {
            NotifyReleaseCallbacks(Point(mouseButton.x, mouseButton.y));
            NotifyClickCallbacks(Point(mouseButton.x, mouseButton.y));
            m_isPressed = false;
        }
    }

    void HandleMouseLeft()
    {
        if (m_isPressed)
        {
            NotifyReleaseCallbacks(Point(mouseButton.x, mouseButton.y));
        }
        m_isPressed = false;
    }

    void HandleMouseMove(const sf::Event::MouseMoveEvent &mouseMove)
    {
        NotifyMoveCallbacks(Point(mouseMove.x, mouseMove.y));
    }

    void HandleMouseLeft()
    {
        if (m_isPressed)
        {
            NotifyReleaseCallbacks(Point(mouseButton.x, mouseButton.y));
        }
        m_isPressed = false;
    }

    void NotifyPressCallbacks(const Point &mousePosition)
    {
        auto callbacksCopy = m_pressCallbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(mousePosition);
            }
        }
    }

    void NotifyReleaseCallbacks(const Point &mousePosition)
    {
        auto callbacksCopy = m_releaseCallbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(mousePosition);
            }
        }
    }

    void NotifyClickCallbacks(const Point &mousePosition)
    {
        auto callbacksCopy = m_clickCallbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(mousePosition);
            }
        }
    }

    void NotifyMoveCallbacks(const Point &mousePosition)
    {
        auto callbacksCopy = m_moveCallbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(mousePosition);
            }
        }
    }

    bool m_isPressed = false;
    std::unordered_map<unsigned, OnClickCallback> m_clickCallbacks;
    std::unordered_map<unsigned, OnPressCallback> m_pressCallbacks;
    std::unordered_map<unsigned, OnReleaseCallback> m_releaseCallbacks;
    std::unordered_map<unsigned, OnMoveCallback> m_moveCallbacks;
};