#pragma once

#include "utils/eventing/event_dispatcher.hpp"

namespace Utils::Eventing::Testing
{
    template <class... ArgTypes>
    class EventDispatcherAccess : public EventDispatcher<ArgTypes...>
    {
    public:
        using EventDispatcher<ArgTypes...>::AddListener;
        using EventDispatcher<ArgTypes...>::RemoveListener;
        using EventDispatcher<ArgTypes...>::RemoveAllListeners;
        using EventDispatcher<ArgTypes...>::GetListenerCount;
        using EventDispatcher<ArgTypes...>::Invoke;
    };
}
