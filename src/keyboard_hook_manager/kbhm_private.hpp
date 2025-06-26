#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <mutex>                // mutex, lock_guard, unique_lock
#include <thread>               // thread
#include <unordered_map>        // unordered_map

#include <global_hotkey/keyboard_hook_manager.hpp>

namespace gbhk
{

namespace kbhook
{

class _KBHMPrivate
{
public:
    _KBHMPrivate();
    virtual ~_KBHMPrivate();

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

protected:
    struct Combine
    {
        constexpr inline Combine() noexcept = default;
        constexpr inline Combine(int nativeKey, KeyState state) noexcept : nativeKey(nativeKey), state(state) {}

        int nativeKey   = 0;
        int state       = 0;

        friend constexpr inline bool operator==(const Combine& lhs, const Combine& rhs) noexcept
        {
            return lhs.nativeKey == rhs.nativeKey && lhs.state == rhs.state;
        }
    };
    friend struct std::hash<Combine>;

    static std::mutex mtx;
    static std::unordered_map<Combine, std::function<void()>> fns;
    static void (*keyPressedCallback)(int);
    static void (*keyReleasedCallback)(int);

    virtual int doBeforeThreadStart();
    virtual int doBeforeThreadEnd();

    virtual int doBeforeLoop();
    virtual int doAfterLoop();
    virtual void eachCycleDo() = 0;

private:
    void workLoop();

    std::condition_variable cvDoBeforeLoopFinished;
    std::condition_variable cvRunning;
    std::atomic<bool> doBeforeLoopFinished;
    std::atomic<bool> shouldClose;
    std::atomic<bool> running;
    std::atomic<size_t> cycleTime;

    std::thread workerThread;
};

} // namespace kbhook

} // namespace gbhk

namespace std
{

template <>
struct hash<gbhk::kbhook::_KBHMPrivate::Combine>
{
    size_t operator()(const gbhk::kbhook::_KBHMPrivate::Combine& obj) const noexcept
    {
        size_t h1 = std::hash<int>()(obj.nativeKey);
        size_t h2 = std::hash<int>()(obj.state);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_HPP
