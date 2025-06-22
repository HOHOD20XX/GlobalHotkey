#include <thread>   // this_thread

#include <global_hotkey/utility.hpp>

#include "utility_private.hpp"

namespace gbhk
{

TimedSleeper::TimedSleeper() : ptr(new _TimedSleeperPrivate()) {}

TimedSleeper::~TimedSleeper() = default;

void TimedSleeper::resetStartTime()
{ ptr->resetStartTime(); }

void TimedSleeper::sleepUntilElapsed(size_t milliseconds) const
{ ptr->sleepUntilElapsed(milliseconds); }

size_t TimedSleeper::elapsedTime() const
{ return ptr->elapsedTime(); }

} // namespace gbhk
