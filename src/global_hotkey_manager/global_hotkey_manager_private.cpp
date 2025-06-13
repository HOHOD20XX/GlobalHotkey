#include "global_hotkey_manager_private.hpp"

namespace gbhk
{

_GlobalHotkeyManagerPrivate::_GlobalHotkeyManagerPrivate() :
    isRunning_(false), shouldClose_(false),
    cycleTime_(_DEFAULT_CYCLE_TIME)
{}

_GlobalHotkeyManagerPrivate::~_GlobalHotkeyManagerPrivate()
{
    end();
}

int _GlobalHotkeyManagerPrivate::start()
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::end()
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::remove(const KeyCombination& kc)
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::removeAll()
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    return RC_SUCCESS;
}

int _GlobalHotkeyManagerPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    return RC_SUCCESS;
}

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
