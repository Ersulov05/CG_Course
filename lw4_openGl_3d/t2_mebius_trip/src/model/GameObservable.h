#pragma once
#include <unordered_map>
#include <functional>

class GameObservable
{
public:
    using OnEndGameCallback = std::function<void()>;

    void OnEndGameSubscribe(void *subscriber, OnEndGameCallback callback)
    {
        m_endGameCallbacks[subscriber] = callback;
    }

    void OnEndGameUnSubscribe(void *subscriber)
    {
        m_endGameCallbacks.erase(subscriber);
    }

protected:
    void NotifyEndGameCallbacks()
    {
        auto callbacks = m_endGameCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
            {
                callback();
            }
        }
    }

private:
    std::unordered_map<void *, OnEndGameCallback> m_endGameCallbacks;
};