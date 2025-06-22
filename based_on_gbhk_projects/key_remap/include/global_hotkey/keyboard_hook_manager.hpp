#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_MANAGER_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_MANAGER_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <functional>   // function
#include <memory>       // unique_ptr

#include "base.hpp"

namespace gbhk
{

namespace kbhook
{

enum KeyState
{
    KS_PRESSED  = 0x01,
    KS_RELEASED = 0x02
};

class _KBHMPrivate;

class GBHK_API KeyboardHookManager final
{
public:
    KeyboardHookManager(const KeyboardHookManager&) = delete;
    KeyboardHookManager& operator=(const KeyboardHookManager&) = delete;

    static KeyboardHookManager& getInstance();

    int start();
    int end();
    int addKeyListener(int nativeKey, KeyState state, const std::function<void()>& fn);
    int removeKeyListener(int nativeKey, KeyState state);
    int removeAllKeyListener();
    int setKeyPressedEvent(void (*fn)(int));
    int setKeyReleasedEvent(void (*fn)(int));
    int unsetKeyPressedEvent();
    int unsetKeyReleasedEvent();
    void setCycleTime(size_t milliseconds);
    bool hasKeyListener(int nativeKey, KeyState state) const;
    bool isRunning() const;

private:
    KeyboardHookManager();
    ~KeyboardHookManager();

    std::unique_ptr<_KBHMPrivate> ptr;
};

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_MANAGER_HPP
