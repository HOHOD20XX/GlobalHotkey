#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <global_hotkey/keyboard_hook_manager.hpp>

#ifdef _GLOBAL_HOTKEY_WIN
    #include "kbhm_private_win.hpp"
#elif defined(_GLOBAL_HOTKEY_MAC)
    #include "kbhm_private_mac.hpp"
#elif defined(_GLOBAL_HOTKEY_LINUX)
    #include "kbhm_private_linux.hpp"
#include "keyboard_hook_manager.hpp"
#endif // _GLOBAL_HOTKEY_WIN

namespace gbhk
{

namespace kbhook
{

KeyboardHookManager& KeyboardHookManager::getInstance()
{
    static KeyboardHookManager instance;
    return instance;
}

KeyboardHookManager::KeyboardHookManager() :
#ifdef _GLOBAL_HOTKEY_WIN
    ptr(new _KBHMPrivateWin())
#elif defined(_GLOBAL_HOTKEY_MAC)
    ptr(new _KBHMPrivateMac())
#elif defined(_GLOBAL_HOTKEY_LINUX)
    ptr(new _KBHMPrivateLinux())
#endif // _GLOBAL_HOTKEY_WIN
{}

KeyboardHookManager::~KeyboardHookManager() = default;

int KeyboardHookManager::run()
{ return ptr->run(); }

int KeyboardHookManager::end()
{ return ptr->end(); }

int KeyboardHookManager::addKeyListener(int nativeKey, KeyState state, const std::function<void ()>& fn)
{ return ptr->addKeyListener(nativeKey, state, fn); }

int KeyboardHookManager::removeKeyListener(int nativeKey, KeyState state)
{ return ptr->removeKeyListener(nativeKey, state); }

int KeyboardHookManager::removeAllKeyListener()
{ return ptr->removeAllKeyListener(); }

int KeyboardHookManager::setKeyPressedCallback(const std::function<void (int)>& fn)
{ return ptr->setKeyPressedCallback(fn); }

int KeyboardHookManager::setKeyReleasedCallback(const std::function<void (int)>& fn)
{ return ptr->setKeyReleasedCallback(fn); }

int KeyboardHookManager::unsetKeyPressedCallback()
{ return ptr->unsetKeyPressedCallback(); }

int KeyboardHookManager::unsetKeyReleasedCallback()
{ return ptr->unsetKeyReleasedCallback(); }

bool KeyboardHookManager::hasKeyListener(int nativeKey, KeyState state) const
{ return ptr->hasKeyListener(nativeKey, state); }

bool KeyboardHookManager::isRunning() const
{ return ptr->isRunning(); }

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
