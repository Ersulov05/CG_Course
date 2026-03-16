#pragma once
#include "../../../common/Point.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <GLFW/glfw3.h>

struct Mouse
{
    Point mousePos;
    int leftButtonState;
};

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

    Mouse GetMouseData(GLFWwindow *window)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Point currentPos(static_cast<float>(xpos), static_cast<float>(ypos));

        int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        return {currentPos, leftButtonState};
    }

    void ProcessEvents(GLFWwindow *window)
    {
        Mouse mouse = GetMouseData(window);

        if (mouse.leftButtonState == GLFW_PRESS)
        {
            HandleMousePress(mouse);
        }

        if (mouse.leftButtonState == GLFW_RELEASE)
        {
            HandleMouseRelease(mouse);
        }

        if (mouse.mousePos != m_mousePosition)
        {
            HandleMouseMove(mouse);
        }

        m_propagate = true;
    }

    unsigned OnPressSubscribe(OnPressCallback onPressCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_pressCallbacks[id] = PrioritizedCallback<OnPressCallback>{onPressCallback, getZOrder, id};
        return id;
    }

    void OnPressUnsubscribe(unsigned subscriptionId)
    {
        m_pressCallbacks.erase(subscriptionId);
    }

    unsigned OnMoveSubscribe(OnMoveCallback onMoveCallback, std::function<unsigned()> getZOrder = nullptr)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_moveCallbacks[id] = PrioritizedCallback<OnMoveCallback>{onMoveCallback, getZOrder, id};
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
        m_releaseCallbacks[id] = PrioritizedCallback<OnReleaseCallback>{onReleaseCallback, getZOrder, id};
        return id;
    }

    void OnReleaseUnsubscribe(unsigned subscriptionId)
    {
        m_releaseCallbacks.erase(subscriptionId);
    }

    void StopPropagation()
    {
        m_propagate = false;
    }

    Point GetPrevMousePos() {
        return m_prevMousePosition;
    }

private:
    bool m_isPressed = false;
    bool m_propagate = true;
    Point m_mousePosition;
    Point m_prevMousePosition;

    std::unordered_map<unsigned, PrioritizedCallback<OnClickCallback>> m_clickCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnPressCallback>> m_pressCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnReleaseCallback>> m_releaseCallbacks;
    std::unordered_map<unsigned, PrioritizedCallback<OnMoveCallback>> m_moveCallbacks;

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

    void HandleMousePress(const Point &mousePos)
    {
        m_isPressed = true;
        NotifyCallbacks(m_pressCallbacks, mousePos);
    }

    void HandleMouseRelease(const Point &mousePos)
    {
        NotifyCallbacks(m_releaseCallbacks, mousePos);
        NotifyCallbacks(m_clickCallbacks, mousePos);
        m_isPressed = false;
    }

    void HandleMouseMove(const Point &mousePos)
    {
        m_mousePosition = mousePos;
        NotifyCallbacks(m_moveCallbacks, m_mousePosition);
    }

    void HandleMousePress(const Mouse &mouse)
    {
        if (!m_isPressed)
        {
            m_isPressed = true;
            NotifyCallbacks(m_pressCallbacks, mouse.mousePos);
        }
    }

    void HandleMouseRelease(const Mouse &mouse)
    {
        if (m_isPressed)
        {
            NotifyCallbacks(m_releaseCallbacks, mouse.mousePos);
            NotifyCallbacks(m_clickCallbacks, mouse.mousePos);
            m_isPressed = false;
        }
    }

    void HandleMouseMove(const Mouse &mouse)
    {
        m_prevMousePosition = m_mousePosition;
        m_mousePosition = mouse.mousePos;
        NotifyCallbacks(m_moveCallbacks, mouse.mousePos);
    }

    void HandleMouseLeft()
    {
        if (m_isPressed)
        {
            NotifyCallbacks(m_releaseCallbacks, m_mousePosition);
        }
        m_isPressed = false;
    }
};