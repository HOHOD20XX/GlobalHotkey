#include "global_hotkey_manager_private.hpp"

namespace gbhk
{

_GlobalHotkeyManagerPrivate::_GlobalHotkeyManagerPrivate() :
    isRunning_(false), shouldClose_(false),
    cycleTime_(_DEFAULT_CYCLE_TIME)
{}

_GlobalHotkeyManagerPrivate::~_GlobalHotkeyManagerPrivate() = default;

int _GlobalHotkeyManagerPrivate::setCycleTime(size_t milliseconds)
{
    cycleTime_ = milliseconds;
    return RC_SUCCESS;
}

bool _GlobalHotkeyManagerPrivate::has(const KeyCombination& kc) const
{
    LOCK_MUTEX(mtx_);
    return funcs_.find(kc) != funcs_.end();
}

bool _GlobalHotkeyManagerPrivate::isAutoRepeat(const KeyCombination& kc) const
{
    LOCK_MUTEX(mtx_);
    if (funcs_.find(kc) != funcs_.end())
        return funcs_.at(kc).first;
    return false;
}

bool _GlobalHotkeyManagerPrivate::isRunning() const
{
    return isRunning_;
}

} // namespace gbhk
