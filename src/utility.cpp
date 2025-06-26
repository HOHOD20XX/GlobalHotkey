#include <thread>   // this_thread

#include <global_hotkey/utility.hpp>

namespace gbhk
{

TimedSleeper::TimedSleeper() : startTime(std::chrono::steady_clock::now()) {}

TimedSleeper::~TimedSleeper() = default;

void TimedSleeper::resetStartTime()
{
    startTime = std::chrono::steady_clock::now();
}

void TimedSleeper::sleepUntilElapsed(size_t milliseconds) const
{
    auto targetTime = startTime + std::chrono::milliseconds(milliseconds);
    std::this_thread::sleep_until(targetTime);
}

size_t TimedSleeper::elapsedTime() const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
    return elapsedTime.count();
}

} // namespace gbhk
