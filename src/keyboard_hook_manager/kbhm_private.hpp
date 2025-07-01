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

    int run();
    int end();
    static int addKeyListener(int nativeKey, KeyState state, const std::function<void ()>& fn);
    static int removeKeyListener(int nativeKey, KeyState state);
    static int removeAllKeyListener();
    static int setKeyPressedCallback(const std::function<void (int)>& fn);
    static int setKeyReleasedCallback(const std::function<void (int)>& fn);
    static int unsetKeyPressedCallback();
    static int unsetKeyReleasedCallback();
    static bool hasKeyListener(int nativeKey, KeyState state);
    bool isRunning() const;

protected:
    /// @return Return a empty value if the `nativeKey` and `state` given is not exists or it is invalid
    /// else return the `callback` value.
    /// @note Thread-safe.
    static std::function<void ()> getKeyListenerCallback(int nativeKey, KeyState state);
    /// @note Thread-safe.
    static std::function<void (int)> getKeyPressedCallback();
    /// @note Thread-safe.
    static std::function<void (int)> getKeyReleasedCallback();

    /// @brief Indicates the worker thread running successfully.
    /// @note The `runningRc` will be set to 'RC_SUCCESS`.
    void setRunSuccess();
    /// @brief Indicates the worker thread running failed.
    /// @note The `runningRc` will be set to 'errorCode`.
    void setRunFail(int errorCode);

    // Some interfaces for subclasses to specialize.

    /// @note This function will be performed before the worker thread is running.
    virtual int doBeforeThreadRun();
    /// @note This function will be performed before the worker thread is end.
    /// @note Specifically, only when this function returns will the semaphore controlling
    /// the thread's end be changed to enable the thread to exit.
    virtual int doBeforeThreadEnd();
    /// @note The specific working logic of the worker thread.
    /// @attention The `setRunSuccess` or `setRunFail` must be performed in the implementation
    /// of this function to indicate whether the work running successfully.
    virtual void work() = 0;

private:
    struct Combination
    {
        constexpr inline Combination() noexcept = default;
        constexpr inline Combination(int nativeKey, KeyState state) noexcept :
            nativeKey(nativeKey), state(state) {}

        int nativeKey   = 0;
        KeyState state  = KS_NONE;

        friend constexpr inline bool operator==(const Combination& lhs, const Combination& rhs) noexcept
        { return lhs.nativeKey == rhs.nativeKey && lhs.state == rhs.state; }
    };
    friend struct std::hash<Combination>;

    enum RunningState
    {
        RS_FREE,
        RS_RUNNING,
        RS_TERMINATE
    };

    static std::mutex mtx;
    static std::unordered_map<Combination, std::function<void ()>> fns;
    static std::function<void (int)> keyPressedCallback;
    static std::function<void (int)> keyReleasedCallback;

    std::condition_variable cvRunningState;
    std::atomic<RunningState> runningState;
    std::atomic<int> runningRc;
    std::atomic<bool> shouldClose;

    std::thread workerThread;
};

} // namespace kbhook

} // namespace gbhk

namespace std
{

template <>
struct hash<gbhk::kbhook::_KBHMPrivate::Combination>
{
    size_t operator()(const gbhk::kbhook::_KBHMPrivate::Combination& obj) const noexcept
    {
        size_t h1 = std::hash<int>()(obj.nativeKey);
        size_t h2 = std::hash<int>()(obj.state);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_HPP
