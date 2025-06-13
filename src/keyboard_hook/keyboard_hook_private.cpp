#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

#define HAS_KEY_LISTENER(x, nativeKey, state) (x.find(nativeKey) != x.end() && x.at(nativeKey).first == state)

namespace gbhk
{

namespace kbhook
{

std::mutex _KeyboardHookPrivate::mtx_;
Map<int, Pair<KeyState, VoidFunc>> _KeyboardHookPrivate::voidFuncs_;
Map<int, Pair<KeyState, ArgFuncArg>> _KeyboardHookPrivate::argFuncArgs_;
KeyEventCallback _KeyboardHookPrivate::keyPressedCallback_    = nullptr;
KeyEventCallback _KeyboardHookPrivate::keyReleasedCallback_   = nullptr;

_KeyboardHookPrivate::_KeyboardHookPrivate() :
    isRunning_(false)
{}

_KeyboardHookPrivate::~_KeyboardHookPrivate() = default;

int _KeyboardHookPrivate::addKeyListener(int nativeKey, KeyState state, VoidFunc func)
{
    LOCK_MUTEX(mtx_);

    if (hasKeyListener(nativeKey, state))
        return RC_EXIST_SAME_VALUE;
    voidFuncs_.insert({ nativeKey, { state, func } });
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::addKeyListener(int nativeKey, KeyState state, ArgFunc func, Arg arg)
{
    LOCK_MUTEX(mtx_);

    if (hasKeyListener(nativeKey, state))
        return RC_EXIST_SAME_VALUE;
    argFuncArgs_.insert({ nativeKey, { state, { func, arg } } });
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::removeKeyListener(int nativeKey, KeyState state)
{
    LOCK_MUTEX(mtx_);

    if (HAS_KEY_LISTENER(voidFuncs_, nativeKey, state))
    {
        voidFuncs_.erase(nativeKey);
        return RC_SUCCESS;
    }
    if (HAS_KEY_LISTENER(argFuncArgs_, nativeKey, state))
    {
        argFuncArgs_.erase(nativeKey);
        return RC_SUCCESS;
    }
    return RC_NO_SPECIFIED_VALUE;
}

int _KeyboardHookPrivate::removeAllKeyListener()
{
    LOCK_MUTEX(mtx_);
    voidFuncs_.clear();
    argFuncArgs_.clear();
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::setKeyPressedEvent(KeyEventCallback func)
{
    LOCK_MUTEX(mtx_);
    keyPressedCallback_ = func;
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::setKeyReleasedEvent(KeyEventCallback func)
{
    LOCK_MUTEX(mtx_);
    keyReleasedCallback_ = func;
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::unsetKeyPressedEvent()
{
    LOCK_MUTEX(mtx_);
    keyPressedCallback_ = nullptr;
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::unsetKeyReleasedEvent()
{
    LOCK_MUTEX(mtx_);
    keyReleasedCallback_ = nullptr;
    return RC_SUCCESS;
}

bool _KeyboardHookPrivate::hasKeyListener(int nativeKey, KeyState state) const
{
    LOCK_MUTEX(mtx_);
    return HAS_KEY_LISTENER(voidFuncs_, nativeKey, state) || HAS_KEY_LISTENER(argFuncArgs_, nativeKey, state);
}

bool _KeyboardHookPrivate::isRunning() const
{
    return isRunning_;
}

void _KeyboardHookPrivate::resetStaticMember_()
{
    removeAllKeyListener();
    unsetKeyPressedEvent();
    unsetKeyReleasedEvent();
}

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
