#include <gtest/gtest.h>

#include <set>

#include "utils/random.hpp"

namespace Utils
{
    TEST(RandomTest, GetIntReturnsValueWithinInclusiveRange)
    {
        for (int i = 0; i < 100; ++i) {
            const int value = GetRandom(-10, 10);
            EXPECT_GE(value, -10);
            EXPECT_LE(value, 10);
        }
    }

    TEST(RandomTest, GetIntReturnsExactValueWhenMinEqualsMax)
    {
        for (int i = 0; i < 20; ++i)
            EXPECT_EQ(GetRandom(7, 7), 7);
    }

    TEST(RandomTest, GetIntProducesMultipleDistinctValuesAcrossRange)
    {
        std::set<int> values;
        for (int i = 0; i < 200; ++i)
            values.insert(GetRandom(0, 100));

        EXPECT_GT(values.size(), 1U);
    }

    TEST(RandomTest, GetFloatReturnsValueWithinInclusiveRange)
    {
        for (int i = 0; i < 100; ++i) {
            const float value = GetRandom(-1.0f, 1.0f);
            EXPECT_GE(value, -1.0f);
            EXPECT_LE(value, 1.0f);
        }
    }

    TEST(RandomTest, GetFloatReturnsExactValueWhenMinEqualsMax)
    {
        for (int i = 0; i < 20; ++i)
            EXPECT_FLOAT_EQ(GetRandom(3.5f, 3.5f), 3.5f);
    }

    TEST(RandomTest, GetFloatProducesMultipleDistinctValuesAcrossRange)
    {
        std::set<float> values;
        for (int i = 0; i < 200; ++i)
            values.insert(GetRandom(0.0f, 1.0f));

        EXPECT_GT(values.size(), 1U);
    }
} // namespace Utils

