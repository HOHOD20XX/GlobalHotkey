#include <thread>   // this_thread

#include <global_hotkey/utility.hpp>

#include "utility_private.hpp"

namespace gbhk
{

TimedSleeper::TimedSleeper() :
    p_(new _TimedSleeperPrivate())
{}

TimedSleeper::~TimedSleeper() = default;

void TimedSleeper::resetStartTime()
{
    p_->resetStartTime();
}

void TimedSleeper::sleepUntilElapsed(size_t milliseconds) const
{
    p_->sleepUntilElapsed(milliseconds);
}

size_t TimedSleeper::elapsedTime() const
{
    return p_->elapsedTime();
}

} // namespace gbhk
