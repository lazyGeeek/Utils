#pragma once

namespace Utils
{
    class NonCopyable
    {
    public:
        NonCopyable() = default;
        virtual ~NonCopyable() = default;

        NonCopyable(const NonCopyable& other) = delete;
        NonCopyable(NonCopyable&& other) = delete;
        NonCopyable& operator=(const NonCopyable& other) = delete;
        NonCopyable& operator=(NonCopyable&& other) = delete;
    };
} // namespace Utils
