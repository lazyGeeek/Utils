#pragma once
#ifndef UTILS_RANDOM_HPP_
#define UTILS_RANDOM_HPP_

#include <random>

namespace Utils
{
    class Random
    {
    public:
        Random() = delete;

        static int GetInt(int min, int max);
        static float GetFloat(float min, float max);

    private:
        static std::default_random_engine m_generator;
    };
}

#endif // UTILS_RANDOM_HPP_
