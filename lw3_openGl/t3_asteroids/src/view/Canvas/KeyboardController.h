#pragma once
#include "../../common/Point.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <bitset>

class KeyboardController
{
public:
    using OnKeyPressedCallback = std::function<void()>;
    using OnKeyReleasedCallback = std::function<void()>;
    using OnKeyHoldCallback = std::function<void(float deltatime)>;

    void Initialize(GLFWwindow *window)
    {
        glfwSetKeyCallback(window, KeyCallbackStatic);
        glfwSetWindowUserPointer(window, this);
    }

    void OnKeyPressedSubscribe(void *subscriber, int key, OnKeyPressedCallback callback)
    {
        m_keyPressedCallbacks[key][subscriber] = callback;
    }

    void OnKeyReleasedSubscribe(void *subscriber, int key, OnKeyReleasedCallback callback)
    {
        m_keyReleasedCallbacks[key][subscriber] = callback;
    }

    void OnKeyHoldSubscribe(void *subscriber, int key, OnKeyHoldCallback callback)
    {
        m_keyHoldCallbacks[key][subscriber] = callback;
    }

    void OnKeyReleasedUnsubscribe(void *subscriber, int key)
    {
        auto keyIt = m_keyReleasedCallbacks.find(key);
        if (keyIt != m_keyReleasedCallbacks.end())
        {
            keyIt->second.erase(subscriber);
            if (keyIt->second.empty())
            {
                m_keyReleasedCallbacks.erase(keyIt);
            }
        }
    }

    void OnKeyPressedUnsubscribe(void *subscriber, int key)
    {
        auto keyIt = m_keyPressedCallbacks.find(key);
        if (keyIt != m_keyPressedCallbacks.end())
        {
            keyIt->second.erase(subscriber);
            if (keyIt->second.empty())
            {
                m_keyPressedCallbacks.erase(keyIt);
            }
        }
    }

    void OnKeyHoldUnsubscribe(void *subscriber, int key)
    {
        auto keyIt = m_keyHoldCallbacks.find(key);
        if (keyIt != m_keyHoldCallbacks.end())
        {
            keyIt->second.erase(subscriber);
            if (keyIt->second.empty())
            {
                m_keyHoldCallbacks.erase(keyIt);
            }
        }
    }

    void Update(float deltatime)
    {
        for (int key = 32; key <= 348; ++key)
        {
            if (IsKeyDown(key))
            {
                NotifyKeyHoldCallbacks(key, deltatime);
            }
        }
    }

    bool IsKeyDown(int key) const
    {
        if (key < 0 || key >= 512)
            return false;

        return m_keys[key];
    }

private:
    std::bitset<512> m_keys;
    std::bitset<512> m_oldKeys;

    std::unordered_map<int, std::unordered_map<void *, OnKeyPressedCallback>> m_keyPressedCallbacks;
    std::unordered_map<int, std::unordered_map<void *, OnKeyReleasedCallback>> m_keyReleasedCallbacks;
    std::unordered_map<int, std::unordered_map<void *, OnKeyHoldCallback>> m_keyHoldCallbacks;

    static void KeyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto *controller = static_cast<KeyboardController *>(glfwGetWindowUserPointer(window));
        if (controller)
        {
            controller->HandleKeyEvent(key, scancode, action, mods);
        }
    }

    void HandleKeyEvent(int key, int scancode, int action, int mods)
    {
        if (key < 0 || key >= 512)
            return;

        m_oldKeys = m_keys;

        if (action == GLFW_PRESS)
        {
            m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_keys[key] = false;
        }

        if (action == GLFW_PRESS)
        {
            NotifyKeyPressedCallbacks(key);
        }
        else if (action == GLFW_RELEASE)
        {
            NotifyKeyReleasedCallbacks(key);
        }
    }

    void NotifyKeyPressedCallbacks(int key)
    {
        auto keyIt = m_keyPressedCallbacks.find(key);
        if (keyIt == m_keyPressedCallbacks.end())
            return;

        auto callbacks = keyIt->second;

        for (auto &[subscriber, callback] : callbacks)
        {
            if (callback)
            {
                callback();
            }
        }
    }

    void NotifyKeyReleasedCallbacks(int key)
    {
        auto keyIt = m_keyReleasedCallbacks.find(key);
        if (keyIt == m_keyReleasedCallbacks.end())
            return;

        auto callbacks = keyIt->second;

        for (auto &[subscriber, callback] : callbacks)
        {
            if (callback)
            {
                callback();
            }
        }
    }

    void NotifyKeyHoldCallbacks(int key, float deltatime)
    {
        auto keyIt = m_keyHoldCallbacks.find(key);
        if (keyIt == m_keyHoldCallbacks.end())
            return;

        auto callbacks = keyIt->second;

        for (auto &[subscriber, callback] : callbacks)
        {
            if (callback)
            {
                callback(deltatime);
            }
        }
    }
};