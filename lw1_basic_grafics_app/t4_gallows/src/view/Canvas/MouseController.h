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

    void OnClickSubscribe(OnClickCallback onClickCallback)
    {
        std::cout << m_callbacks.size() << std::endl;
        m_callbacks.push_back(onClickCallback);
        std::cout << m_callbacks.size() << std::endl;
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
        std::cout << m_callbacks.size() << std::endl;
        int i = 1;
        for (const auto &callback : m_callbacks)
        {
            if (callback)
            {
                std::cout << "click" + i++ << std::endl;
                callback(mousePosition);
            }
        }
    }

    bool m_isPressed = false;
    std::vector<OnClickCallback> m_callbacks;
};