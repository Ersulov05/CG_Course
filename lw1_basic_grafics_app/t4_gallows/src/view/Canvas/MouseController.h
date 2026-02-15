#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "../Common/Point.h"

class MouseController
{
public:
    using OnClickCallback = std::function<void(const Point &)>;

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

        case sf::Event::MouseLeft:
            HandleMouseLeft();
            break;

        default:
            break;
        }
    }

    unsigned OnClickSubscribe(OnClickCallback onClickCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_callbacks[id] = onClickCallback;
        return id;
    }

    void OnClickUnsubscribe(unsigned subscriptionId)
    {
        m_callbacks.erase(subscriptionId);
    }

    void ClearCallbacks()
    {
        m_callbacks.clear();
    }

private:
    void HandleMousePress(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left)
        {
            m_isPressed = true;
        }
    }

    void HandleMouseRelease(const sf::Event::MouseButtonEvent &mouseButton)
    {
        if (mouseButton.button == sf::Mouse::Left && m_isPressed)
        {
            NotifyCallbacks(Point(mouseButton.x, mouseButton.y));
            m_isPressed = false;
        }
    }

    void HandleMouseLeft()
    {
        m_isPressed = false;
    }

    void NotifyCallbacks(const Point &mousePosition)
    {
        auto callbacksCopy = m_callbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(mousePosition);
            }
        }
    }

    bool m_isPressed = false;
    std::unordered_map<unsigned, OnClickCallback> m_callbacks;
};