#include <gtest/gtest.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "event_dispatcher_access.hpp"

using Utils::Eventing::ListenerID;
using Utils::Eventing::Testing::EventDispatcherAccess;

TEST(EventDispatcherTest, AddListenerReturnsIncrementingIds)
{
    EventDispatcherAccess<int> dispatcher;

    const ListenerID first = dispatcher.AddListener([](int) { });
    const ListenerID second = dispatcher.AddListener([](int) { });

    EXPECT_EQ(first, 0U);
    EXPECT_EQ(second, 1U);
    EXPECT_EQ(dispatcher.GetListenerCount(), 2U);
}

TEST(EventDispatcherTest, InvokeCallsRegisteredListenersWithArguments)
{
    EventDispatcherAccess<int, std::string> dispatcher;

    int intArg = 0;
    std::string stringArg;

    dispatcher.AddListener([&](int value, const std::string& text) {
        intArg = value;
        stringArg = text;
    });

    dispatcher.Invoke(42, "hello");

    EXPECT_EQ(intArg, 42);
    EXPECT_EQ(stringArg, "hello");
}

TEST(EventDispatcherTest, InvokeCallsAllRegisteredListeners)
{
    EventDispatcherAccess<int> dispatcher;

    int total = 0;
    dispatcher.AddListener([&](int value) { total += value; });
    dispatcher.AddListener([&](int value) { total += value; });
    dispatcher.AddListener([&](int value) { total += value; });

    dispatcher.Invoke(5);

    EXPECT_EQ(total, 15);
}

TEST(EventDispatcherTest, RemoveListenerRemovesOnlyRequestedListener)
{
    EventDispatcherAccess<int> dispatcher;

    int firstCount = 0;
    int secondCount = 0;

    const ListenerID firstId =
        dispatcher.AddListener([&](int) { ++firstCount; });
    dispatcher.AddListener([&](int) { ++secondCount; });

    EXPECT_TRUE(dispatcher.RemoveListener(firstId));
    EXPECT_FALSE(dispatcher.RemoveListener(firstId));
    EXPECT_EQ(dispatcher.GetListenerCount(), 1U);

    dispatcher.Invoke(0);

    EXPECT_EQ(firstCount, 0);
    EXPECT_EQ(secondCount, 1);
}

TEST(EventDispatcherTest, RemoveAllListenersClearsCallbacks)
{
    EventDispatcherAccess<int> dispatcher;

    dispatcher.AddListener([](int) { });
    dispatcher.AddListener([](int) { });
    EXPECT_EQ(dispatcher.GetListenerCount(), 2U);

    dispatcher.RemoveAllListeners();

    EXPECT_EQ(dispatcher.GetListenerCount(), 0U);
}

TEST(EventDispatcherTest, InvokeIsSafeWithConcurrentAddAndRemove)
{
    EventDispatcherAccess<int> dispatcher;
    std::atomic<int> invokeCount { 0 };

    dispatcher.AddListener([&](int) { invokeCount.fetch_add(1); });

    std::vector<std::thread> threads;
    threads.reserve(8);

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < 50; ++j)
                dispatcher.AddListener([](int) { });
        });
        threads.emplace_back([&] {
            for (int j = 0; j < 50; ++j)
                dispatcher.RemoveListener(static_cast<ListenerID>(j));
        });
    }

    for (int i = 0; i < 100; ++i)
        dispatcher.Invoke(0);

    for (auto& thread : threads)
        thread.join();

    EXPECT_GE(invokeCount.load(), 100);
}
