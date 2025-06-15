#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <global_hotkey/keyboard_hook.hpp>

#ifdef _GLOBAL_HOTKEY_WIN
    #include "keyboard_hook_private_win.hpp"
#elif defined(_GLOBAL_HOTKEY_MAC)
    #include "keyboard_hook_private_mac.hpp"
#elif defined(_GLOBAL_HOTKEY_LINUX)
    #include "keyboard_hook_private_linux.hpp"
#endif // _GLOBAL_HOTKEY_WIN

namespace gbhk
{

namespace kbhook
{

KeyboardHook& KeyboardHook::getInstance()
{
    static KeyboardHook instance;
    return instance;
}

KeyboardHook::KeyboardHook() :
#ifdef _GLOBAL_HOTKEY_WIN
    p_(new _KeyboardHookPrivateWin())
#elif defined(_GLOBAL_HOTKEY_MAC)
    p_(new _KeyboardHookPrivateMac())
#elif defined(_GLOBAL_HOTKEY_LINUX)
    p_(new _KeyboardHookPrivateLinux())
#endif // _GLOBAL_HOTKEY_WIN
{}

KeyboardHook::~KeyboardHook() = default;

int KeyboardHook::start()
{ return p_->start(); }

int KeyboardHook::end()
{ return p_->end(); }

int KeyboardHook::addKeyListener(int nativeKey, KeyState state, const std::function<void()>& fn)
{ return p_->addKeyListener(nativeKey, state, fn); }

int KeyboardHook::addKeyListener(int nativeKey, KeyState state, std::function<void()>&& fn)
{ return p_->addKeyListener(nativeKey, state, std::move(fn)); }

int KeyboardHook::removeKeyListener(int nativeKey, KeyState state)
{ return p_->removeKeyListener(nativeKey, state); }

int KeyboardHook::removeAllKeyListener()
{ return p_->removeAllKeyListener(); }

int KeyboardHook::setKeyPressedEvent(void (*fn)(int))
{ return p_->setKeyPressedEvent(fn); }

int KeyboardHook::setKeyReleasedEvent(void (*fn)(int))
{ return p_->setKeyReleasedEvent(fn); }

int KeyboardHook::unsetKeyPressedEvent()
{ return p_->unsetKeyPressedEvent(); }

int KeyboardHook::unsetKeyReleasedEvent()
{ return p_->unsetKeyReleasedEvent(); }

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
