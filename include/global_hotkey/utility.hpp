#ifndef GLOBAL_HOTKEY_UTILITY_HPP
#define GLOBAL_HOTKEY_UTILITY_HPP

#include <chrono>   // chrono

#include "base.hpp"

namespace gbhk
{

class GBHK_API TimedSleeper final
{
public:
    TimedSleeper();
    ~TimedSleeper();

    /// @brief Reset the start time of the sleeper.
    void resetStartTime();
    /// @brief Sleep until the specified number of milliseconds has elapsed since the start time.
    void sleepUntilElapsed(size_t milliseconds) const;
    /// @brief Get the elapsed time (in milliseconds) since the start time.
    size_t elapsedTime() const;

private:
    std::chrono::steady_clock::time_point startTime;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_UTILITY_HPP
