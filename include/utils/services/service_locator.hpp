#pragma once
#ifndef UTILS_SERVICES_SERVICE_LOCATOR_HPP_
#define UTILS_SERVICES_SERVICE_LOCATOR_HPP_

#include <iostream>
#include <memory>
#include <unordered_map>
#include <stdexcept>

#include "service_interface.hpp"

namespace Utils::Services
{
    class ServiceLocator
    {
    public:
        template<typename T, typename... Args>
        static T& Provide(Args&&... args)
        {
            static_assert(std::is_base_of<IService, T>::value, "T must inherit from IService");

            size_t hash = typeid(T).hash_code();

            if (!m_services.count(hash))
                m_services.emplace(hash, std::make_shared<T>(args...));

            return static_cast<T&>(*m_services.find(hash)->second.get());
        }

        template<typename T>
        static T& Get()
        {
            static_assert(std::is_base_of<IService, T>::value, "T must inherit from IService");

            if (!m_services.count(typeid(T).hash_code()))
                throw std::runtime_error("Service does not exist");

            return static_cast<T&>(*m_services[typeid(T).hash_code()].get());
        }

        template<typename T>
        static void UnregisterService()
        {
            static_assert(std::is_base_of<IService, T>::value, "T must inherit from IService");

            size_t hash = typeid(T).hash_code();

            if (m_services.count(hash))
                m_services.erase(hash);
        }

        private:
            static inline std::unordered_map<size_t, std::shared_ptr<IService>> m_services;
    };
}

#endif // UTILS_SERVICES_SERVICE_LOCATOR_HPP_
