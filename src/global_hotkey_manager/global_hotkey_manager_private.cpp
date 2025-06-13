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
    return voidFuncs_.find(kc) != voidFuncs_.end() || argFuncArgs_.find(kc) != argFuncArgs_.end();
}

bool _GlobalHotkeyManagerPrivate::isAutoRepeat(const KeyCombination& kc) const
{
    LOCK_MUTEX(mtx_);

    if (voidFuncs_.find(kc) != voidFuncs_.end())
        return voidFuncs_.at(kc).first;
    else if (argFuncArgs_.find(kc) != argFuncArgs_.end())
        return argFuncArgs_.at(kc).first;
    else
        return false;
}

bool _GlobalHotkeyManagerPrivate::isRunning() const
{
    return isRunning_;
}

} // namespace gbhk
