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

enum KeyState : int
{
    KS_NONE     = 0x00,
    KS_PRESSED  = 0x01,
    KS_RELEASED = 0x02
};

class _KBHMPrivate;

class GBHK_API KeyboardHookManager final
{
public:
    static KeyboardHookManager& getInstance();

    /// @brief Run the Keyboard Hook Manager.
    int run();
    /// @brief End the Keyboard Hook Manager.
    int end();
    /// @brief Add a key listener to the Keyboard Hook Manager.
    /// @param nativeKey The native key code you will listen.
    /// @param state The key state you will listen.
    /// @param fn The callback function triggered when the listener is actived.
    int addKeyListener(int nativeKey, KeyState state, const std::function<void ()>& fn);
    /// @brief Remove a key listener from the Keyboard Hook Manager.
    /// @param nativeKey The native key code of the listener.
    /// @param state The key state of the listener.
    int removeKeyListener(int nativeKey, KeyState state);
    /// @brief Remove all listener in the Keyboard Hook Manager.
    int removeAllKeyListener();
    /// @brief Set the callback function triggered when has a key be pressed.
    int setKeyPressedCallback(const std::function<void (int)>& fn);
    /// @brief Set the callback function triggered when has a key be released.
    int setKeyReleasedCallback(const std::function<void (int)>& fn);
    /// @brief Unset the callback function triggered when has a key be pressed.
    int unsetKeyPressedCallback();
    /// @brief Unset the callback function triggered when has a key be released.
    int unsetKeyReleasedCallback();
    /// @brief Check whether the listener is exists in the Keyboard Hook Manager.
    /// @param nativeKey The native key code of the listener.
    /// @param state The key state of the listener.
    bool hasKeyListener(int nativeKey, KeyState state) const;
    /// @brief Whether the Keyboard Hook Manager is running.
    bool isRunning() const;

private:
    KeyboardHookManager();
    ~KeyboardHookManager();
    KeyboardHookManager(const KeyboardHookManager&) = delete;
    KeyboardHookManager& operator=(const KeyboardHookManager&) = delete;

    std::unique_ptr<_KBHMPrivate> ptr;
};

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_MANAGER_HPP
