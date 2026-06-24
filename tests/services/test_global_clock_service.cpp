#include <gtest/gtest.h>

#include "utils/services/service_locator.hpp"
#include "utils/time/global_clock.hpp"

namespace Utils::Services
{
    class GlobalClockServiceTest : public ::testing::Test
    {
    protected:
        void TearDown() override
        {
            ServiceLocator::UnregisterService<Time::GlobalClock>();
        }
    };

    TEST_F(GlobalClockServiceTest,
           CanBeRegisteredAndRetrievedThroughServiceLocator)
    {
        auto& provided = ServiceLocator::Provide<Time::GlobalClock>();
        auto& fetched = ServiceLocator::Get<Time::GlobalClock>();

        EXPECT_EQ(&provided, &fetched);
    }

    TEST(GlobalClockTest, IsNonCopyable)
    {
        static_assert(!std::is_copy_constructible_v<Time::GlobalClock>);
        static_assert(!std::is_copy_assignable_v<Time::GlobalClock>);
        static_assert(!std::is_move_constructible_v<Time::GlobalClock>);
        static_assert(!std::is_move_assignable_v<Time::GlobalClock>);
    }
} // namespace Utils::ServiceLocator

