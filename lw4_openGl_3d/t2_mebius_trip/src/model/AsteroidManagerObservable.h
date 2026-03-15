#pragma once
#include "./AsteroidModel.h"
#include <unordered_map>
#include <functional>

class AsteroidManagerObservable
{
public:
    using OnResetCallback = std::function<void()>;
    using OnAddCallback = std::function<void(const std::shared_ptr<AsteroidModel> &)>;
    using OnRemoveCallback = std::function<void(const std::shared_ptr<AsteroidModel> &)>;

    void OnResetSubscribe(void *subscriber, OnResetCallback callback)
    {
        m_resetCallbacks[subscriber] = callback;
    }

    void OnResetUnSubscribe(void *subscriber)
    {
        m_resetCallbacks.erase(subscriber);
    }

    void OnAddSubscribe(void *subscriber, OnAddCallback callback)
    {
        m_addedCallbacks[subscriber] = callback;
    }

    void OnAddUnSubscribe(void *subscriber)
    {
        m_addedCallbacks.erase(subscriber);
    }

    void OnRemoveSubscribe(void *subscriber, OnRemoveCallback callback)
    {
        m_removedCallbacks[subscriber] = callback;
    }

    void OnRemoveUnSubscribe(void *subscriber)
    {
        m_removedCallbacks.erase(subscriber);
    }

protected:
    void NotifyResetCallbacks()
    {
        auto callbacks = m_resetCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
            {
                callback();
            }
        }
    }

    void NotifyAsteroidAdded(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        auto callbacks = m_addedCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
                callback(asteroid);
        }
    }

    void NotifyAsteroidRemoved(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        auto callbacks = m_removedCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
                callback(asteroid);
        }
    }

private:
    std::unordered_map<void *, OnResetCallback> m_resetCallbacks;
    std::unordered_map<void *, OnAddCallback> m_addedCallbacks;
    std::unordered_map<void *, OnRemoveCallback> m_removedCallbacks;
};