#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <atomic>   // atomic
#include <mutex>    // mutex, lock_guard

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/keyboard_hook.hpp>

#define LOCK_MUTEX(mtx) std::lock_guard<std::mutex> lock(mtx)

namespace gbhk
{

namespace kbhook
{

class _KeyboardHookPrivate
{
public:
    _KeyboardHookPrivate();
    virtual ~_KeyboardHookPrivate();

    virtual int start() = 0;
    virtual int end() = 0;
    int addKeyListener(int nativeKey, KeyState state, VoidFunc func);
    int addKeyListener(int nativeKey, KeyState state, ArgFunc func, Arg arg);
    int removeKeyListener(int nativeKey, KeyState state);
    int removeAllKeyListener();
    int setKeyPressedEvent(KeyEventCallback func);
    int setKeyReleasedEvent(KeyEventCallback func);
    int unsetKeyPressedEvent();
    int unsetKeyReleasedEvent();

    bool hasKeyListener(int nativeKey, KeyState state) const;
    bool isRunning() const;

protected:
    void resetStaticMember_();

    static std::mutex mtx_;
    static Map<int, Pair<KeyState, VoidFunc>> voidFuncs_;
    static Map<int, Pair<KeyState, ArgFuncArg>> argFuncArgs_;
    static KeyEventCallback keyPressedCallback_;
    static KeyEventCallback keyReleasedCallback_;

    std::atomic<bool> isRunning_;
};

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP
