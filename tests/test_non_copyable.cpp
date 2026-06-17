#include <gtest/gtest.h>
#include <type_traits>

#include "utils/non_copyable.hpp"

namespace
{
    class ConcreteNonCopyable : public Utils::NonCopyable
    {
    public:
        int value = 0;
    };
}

TEST(NonCopyableTest, CopyAndMoveOperationsAreDeleted)
{
    static_assert(!std::is_copy_constructible_v<Utils::NonCopyable>);
    static_assert(!std::is_copy_assignable_v<Utils::NonCopyable>);
    static_assert(!std::is_move_constructible_v<Utils::NonCopyable>);
    static_assert(!std::is_move_assignable_v<Utils::NonCopyable>);

    static_assert(!std::is_copy_constructible_v<ConcreteNonCopyable>);
    static_assert(!std::is_copy_assignable_v<ConcreteNonCopyable>);
    static_assert(!std::is_move_constructible_v<ConcreteNonCopyable>);
    static_assert(!std::is_move_assignable_v<ConcreteNonCopyable>);
}

TEST(NonCopyableTest, DerivedTypeCanBeDefaultConstructedAndDestroyed)
{
    ConcreteNonCopyable instance;
    instance.value = 42;
    EXPECT_EQ(instance.value, 42);
}
