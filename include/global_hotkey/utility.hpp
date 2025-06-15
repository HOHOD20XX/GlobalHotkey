#ifndef GLOBAL_HOTKEY_UTILITY_HPP
#define GLOBAL_HOTKEY_UTILITY_HPP

#include <memory>   // unique_ptr

#include "base.hpp"

namespace gbhk
{

class _TimedSleeperPrivate;

class GBHK_API TimedSleeper final
{
public:
    TimedSleeper();
    ~TimedSleeper();

    /// @brief Reset the start time of the timer.
    void resetStartTime();
    /// @brief Sleep until the specified number of milliseconds has elapsed since the start time.
    void sleepUntilElapsed(size_t milliseconds) const;
    /// @brief Get the elapsed time (in milliseconds) since the start time.
    size_t elapsedTime() const;

private:
    std::unique_ptr<_TimedSleeperPrivate> p_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_UTILITY_HPP
