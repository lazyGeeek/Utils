#pragma once
#include <random>

namespace Utils
{
    template <std::floating_point T>
    T GetRandom(T min, T max)
    {
        static thread_local std::mt19937 rng(std::random_device {}());
        std::uniform_real_distribution<T> dist(min, max);
        return dist(rng);
    }

    template <std::integral T>
    T GetRandom(T min, T max)
    {
        static thread_local std::mt19937 rng(std::random_device {}());
        std::uniform_int_distribution<T> dist(min, max);
        return dist(rng);
    }
} // namespace Utils

