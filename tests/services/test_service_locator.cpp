#include <gtest/gtest.h>

#include "utils/services/service_locator.hpp"

namespace Utils::Services
{
    namespace
    {
        class TestServiceA : public IService
        {
        public:
            explicit TestServiceA(int value = 0) : m_value(value)
            { }

            int Value() const
            {
                return m_value;
            }

        private:
            int m_value;
        };

        class TestServiceB : public IService
        {
        };
    } // namespace

    class ServiceLocatorTest : public ::testing::Test
    {
    protected:
        void TearDown() override
        {
            ServiceLocator::UnregisterService<TestServiceA>();
            ServiceLocator::UnregisterService<TestServiceB>();
        }
    };

    TEST_F(ServiceLocatorTest, ProvideCreatesServiceAndGetReturnsSameInstance)
    {
        auto& provided = ServiceLocator::Provide<TestServiceA>(123);
        auto& fetched = ServiceLocator::Get<TestServiceA>();

        EXPECT_EQ(provided.Value(), 123);
        EXPECT_EQ(fetched.Value(), 123);
        EXPECT_EQ(&provided, &fetched);
    }

    TEST_F(ServiceLocatorTest, ProvideReturnsExistingInstanceForSameType)
    {
        auto& first = ServiceLocator::Provide<TestServiceA>(1);
        auto& second = ServiceLocator::Provide<TestServiceA>(999);

        EXPECT_EQ(&first, &second);
        EXPECT_EQ(second.Value(), 1);
    }

    TEST_F(ServiceLocatorTest, GetThrowsWhenServiceWasNotProvided)
    {
        EXPECT_THROW(Utils::Services::ServiceLocator::Get<TestServiceA>(),
                     std::runtime_error);
    }

    TEST_F(ServiceLocatorTest, UnregisterServiceRemovesService)
    {
        ServiceLocator::Provide<TestServiceA>(10);
        ServiceLocator::UnregisterService<TestServiceA>();

        EXPECT_THROW(ServiceLocator::Get<TestServiceA>(), std::runtime_error);
    }

    TEST_F(ServiceLocatorTest, ServicesAreStoredSeparatelyByType)
    {
        ServiceLocator::Provide<TestServiceA>(5);
        ServiceLocator::Provide<TestServiceB>();

        EXPECT_EQ(ServiceLocator::Get<TestServiceA>().Value(), 5);
        EXPECT_NO_THROW(ServiceLocator::Get<TestServiceB>());
    }

    TEST_F(ServiceLocatorTest, UnregisterServiceIsSafeWhenServiceDoesNotExist)
    {
        EXPECT_NO_THROW(ServiceLocator::UnregisterService<TestServiceA>());
    }
} // namespace Utils::Services

