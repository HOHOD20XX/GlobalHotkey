#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP

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

class _KeyboardHookPrivate;

class GBHK_API KeyboardHook final
{
public:
    KeyboardHook(const KeyboardHook&) = delete;
    KeyboardHook& operator=(const KeyboardHook&) = delete;

    static KeyboardHook& getInstance();

    int start();
    int end();
    int addKeyListener(int nativeKey, KeyState state, const std::function<void()>& fn);
    int addKeyListener(int nativeKey, KeyState state, std::function<void()>&& fn);
    int removeKeyListener(int nativeKey, KeyState state);
    int removeAllKeyListener();
    int setKeyPressedEvent(void (*fn)(int));
    int setKeyReleasedEvent(void (*fn)(int));
    int unsetKeyPressedEvent();
    int unsetKeyReleasedEvent();

    bool hasKeyListener(int nativeKey, KeyState state) const;
    bool isRunning() const;

private:
    KeyboardHook();
    ~KeyboardHook();

    std::unique_ptr<_KeyboardHookPrivate> p_;
};

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
