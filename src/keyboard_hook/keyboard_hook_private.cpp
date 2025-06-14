#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

namespace gbhk
{

namespace kbhook
{

std::mutex _KeyboardHookPrivate::mtx_;
std::unordered_map<_KeyboardHookPrivate::Combine, std::function<void()>, _KeyboardHookPrivate::Combine::Hash>
_KeyboardHookPrivate::funcs_;
void (*_KeyboardHookPrivate::keyPressedCallback_)(int)      = nullptr;
void (*_KeyboardHookPrivate::keyReleasedCallback_)(int)     = nullptr;

_KeyboardHookPrivate::_KeyboardHookPrivate() :
    isRunning_(false)
{}

_KeyboardHookPrivate::~_KeyboardHookPrivate() = default;

int _KeyboardHookPrivate::addKeyListener(int nativeKey, KeyState state, const std::function<void()>& func)
{
    if (!func)                              return RC_INVALID_VALUE;
    if (hasKeyListener(nativeKey, state))   return RC_EXIST_SAME_VALUE;

    LOCK_MUTEX(mtx_);
    funcs_.insert({ { nativeKey, state }, func });
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::addKeyListener(int nativeKey, KeyState state, std::function<void()>&& func)
{
    if (!func)                              return RC_INVALID_VALUE;
    if (hasKeyListener(nativeKey, state))   return RC_EXIST_SAME_VALUE;

    LOCK_MUTEX(mtx_);
    funcs_.insert({ { nativeKey, state }, std::move(func) });
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::removeKeyListener(int nativeKey, KeyState state)
{
    if (!hasKeyListener(nativeKey, state))
        return RC_NO_SPECIFIED_VALUE;

    LOCK_MUTEX(mtx_);
    funcs_.erase({ nativeKey, state });
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::removeAllKeyListener()
{
    LOCK_MUTEX(mtx_);
    funcs_.clear();
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::setKeyPressedEvent(void (*func)(int))
{
    if (func == nullptr)
        return RC_INVALID_VALUE;

    LOCK_MUTEX(mtx_);
    keyPressedCallback_ = func;
    return RC_SUCCESS;
}

int _KeyboardHookPrivate::setKeyReleasedEvent(void (*func)(int))
{
    if (func == nullptr)
        return RC_INVALID_VALUE;

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
    return funcs_.find({ nativeKey, state }) != funcs_.end();
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
