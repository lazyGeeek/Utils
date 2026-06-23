#pragma once

#include <cstdint>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace Utils::Eventing
{
    using ListenerID = uint64_t;

    template <class... ArgTypes>
    class EventDispatcher
    {
    public:
        using Callback = std::function<void(ArgTypes&&...)>;

        virtual ~EventDispatcher() = default;

        virtual ListenerID AddListener(Callback callback)
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            ListenerID listenerID = m_availableListenerID++;
            m_callbacks[listenerID] = std::move(callback);
            return listenerID;
        }

        virtual bool RemoveListener(ListenerID listenerID)
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            return m_callbacks.erase(listenerID) != 0;
        }

    protected:
        virtual void RemoveAllListeners()
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            m_callbacks.clear();
        }

        virtual uint64_t GetListenerCount()
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            return m_callbacks.size();
        }

        virtual void Invoke(ArgTypes&&... args)
        {
            std::vector<Callback> callbacks;
            {
                std::lock_guard<std::mutex> lk(m_mutex);
                callbacks.reserve(m_callbacks.size());

                for (auto& cb : m_callbacks)
                    callbacks.push_back(cb.second);
            }

            for (auto& fc : callbacks)
                fc(std::forward<ArgTypes>(args)...);
        }

    private:
        std::unordered_map<ListenerID, Callback> m_callbacks;
        ListenerID m_availableListenerID = 0;
        std::mutex m_mutex;
    };
} // namespace Utils::Eventing

