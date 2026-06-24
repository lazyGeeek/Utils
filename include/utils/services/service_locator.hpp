#pragma once

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "service_interface.hpp"

namespace Utils::Services
{
    template <class T>
    concept Service = std::is_base_of_v<IService, T>;

    class ServiceLocator
    {
    public:
        template <Service S, class... Args>
        static S& Provide(Args&&... args)
        {
            const std::type_index key { typeid(S) };

            auto [it, inserted] = m_services.try_emplace(
                key, std::make_shared<S>(std::forward<Args>(args)...));

            return static_cast<S&>(*it->second);
        }

        template <Service S>
        static S& Get()
        {
            const std::type_index key { typeid(S) };

            auto it = m_services.find(key);
            if (it == m_services.end())
                throw std::runtime_error("Service does not exist");

            return static_cast<S&>(*it->second);
        }

        template <Service S>
        static void UnregisterService()
        {
            m_services.erase(typeid(S));
        }

    private:
        static inline std::unordered_map<std::type_index,
                                         std::shared_ptr<IService>>
            m_services;
    };
} // namespace Utils::Services

