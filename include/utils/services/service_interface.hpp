#pragma once
#ifndef UTILS_SERVICES_SERVICE_INTERFACE_HPP_
#define UTILS_SERVICES_SERVICE_INTERFACE_HPP_

#include <typeindex>

namespace Utils::Services
{
    class IService
    {
    public:
        virtual ~IService() = default;
    };
}

#endif // UTILS_SERVICES_SERVICE_INTERFACE_HPP_
