#include <thread>   // this_thread

#include "utility_private.hpp"

namespace gbhk
{

_TimedSleeperPrivate::_TimedSleeperPrivate() : startTime(std::chrono::steady_clock::now()) {}

void _TimedSleeperPrivate::resetStartTime()
{
    startTime = std::chrono::steady_clock::now();
}

void _TimedSleeperPrivate::sleepUntilElapsed(size_t milliseconds) const
{
    auto targetTime = startTime + std::chrono::milliseconds(milliseconds);
    std::this_thread::sleep_until(targetTime);
}

size_t _TimedSleeperPrivate::elapsedTime() const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
    return elapsedTime.count();
}

} // namespace gbhk
