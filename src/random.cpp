#include "utils/random.hpp"

namespace Utils
{
    std::default_random_engine Random::m_generator;

    int Random::GetInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(m_generator);
    }

    float Random::GetFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(m_generator);
    }
}
