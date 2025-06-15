#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <atomic>               // atomic
#include <functional>           // function
#include <mutex>                // mutex, lock_guard, unique_lock
#include <unordered_map>        // unordered_map

#include <global_hotkey/keyboard_hook.hpp>
#include <global_hotkey/return_code.hpp>

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

protected:
    struct Combine
    {
        Combine() = default;
        Combine(int nativeKey, KeyState state) : nativeKey(nativeKey), state(state) {}

        int nativeKey   = 0;
        int state       = 0;

        struct Hash
        {
            size_t operator()(const Combine& obj) const
            {
                size_t h1 = std::hash<int>()(obj.nativeKey);
                size_t h2 = std::hash<int>()(obj.state);
                return h1 ^ (h2 << 1);
            }
        };

        friend bool operator==(const Combine& lhs, const Combine& rhs)
        {
            return lhs.nativeKey == rhs.nativeKey && lhs.state == rhs.state;
        }
    };

    void resetStaticMember_();

    static std::mutex mtx_;
    static std::unordered_map<Combine, std::function<void()>, Combine::Hash> fns_;
    static void (*keyPressedCallback_)(int);
    static void (*keyReleasedCallback_)(int);

    std::atomic<bool> isRunning_;
};

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_HPP
