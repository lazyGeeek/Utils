#include "utils/random.hpp"

namespace Utils
{
    int Random::GetInt(int min, int max)
    {
        std::random_device r;
        std::default_random_engine generator(r());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    float Random::GetFloat(float min, float max)
    {
        std::random_device r;
        std::default_random_engine generator(r());
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
}
