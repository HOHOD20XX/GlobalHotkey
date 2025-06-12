#ifndef GLOBAL_HOTKEY_UTILITY_PRIVATE_HPP
#define GLOBAL_HOTKEY_UTILITY_PRIVATE_HPP

#include <chrono>   // chrono

namespace gbhk
{

class _TimedSleeperPrivate final
{
public:
    _TimedSleeperPrivate();
    ~_TimedSleeperPrivate();

    void resetStartTime();
    void sleepUntilElapsed(size_t milliseconds) const;
    size_t elapsedTime() const;

private:
    std::chrono::steady_clock::time_point startTime_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_UTILITY_PRIVATE_HPP
