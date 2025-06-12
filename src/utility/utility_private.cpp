#include <thread>   // this_thread

#include "utility_private.hpp"

namespace gbhk
{

_TimedSleeperPrivate::_TimedSleeperPrivate() :
    startTime_(std::chrono::steady_clock::now())
{}

_TimedSleeperPrivate::~_TimedSleeperPrivate() = default;

void _TimedSleeperPrivate::resetStartTime()
{
    startTime_ = std::chrono::steady_clock::now();
}

void _TimedSleeperPrivate::sleepUntilElapsed(size_t milliseconds) const
{
    auto targetTime = startTime_ + std::chrono::milliseconds(milliseconds);
    std::this_thread::sleep_until(targetTime);
}

size_t _TimedSleeperPrivate::elapsedTime() const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime_);
    return elapsedTime.count();
}

} // namespace gbhk
