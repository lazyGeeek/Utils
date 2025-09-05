#pragma once
#ifndef UTILS_EVENTING_EVENT_HPP_
#define UTILS_EVENTING_EVENT_HPP_

#include <functional>
#include <cstdint>
#include <unordered_map>

namespace Utils::Eventing
{
    using ListenerID = uint64_t;

    template<class... ArgTypes>
    class Event
    {
    public:
        using Callback = std::function<void(ArgTypes...)>;

        ListenerID operator+=(Callback callback)
        {
            return AddListener(callback);
        }

        bool operator-=(ListenerID listenerID)
        {
            return RemoveListener(listenerID);
        }

        ListenerID AddListener(Callback callback)
        {
            ListenerID listenerID = m_availableListenerID++;
            m_callbacks.emplace(listenerID, callback);
            return listenerID;
        }

        bool RemoveListener(ListenerID listenerID)
        {
            return m_callbacks.erase(listenerID) != 0;
        }

        void RemoveAllListeners()
        {
            m_callbacks.clear();
        }

        uint64_t GetListenerCount()
        {
            return m_callbacks.size();
        }

        void Invoke(ArgTypes... args)
        {
            for (auto const& [key, value] : m_callbacks)
                value(args...);
        }

    private:
        std::unordered_map<ListenerID, Callback> m_callbacks;
        ListenerID m_availableListenerID = 0;
    };
}

#endif // UTILS_EVENTING_EVENT_HPP_
