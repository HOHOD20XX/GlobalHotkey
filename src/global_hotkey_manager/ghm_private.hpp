#ifndef GLOBAL_HOTKEY_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_GHM_PRIVATE_HPP

#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <functional>           // function
#include <mutex>                // mutex, lock_guard, unique_lock
#include <thread>               // thread, thread::id
#include <unordered_map>        // unordered_map
#include <vector>               // vector

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

class _GHMPrivate
{
public:
    _GHMPrivate();
    virtual ~_GHMPrivate();

    int start();
    int end();
    int add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat);
    int remove(const KeyCombination& kc);
    int removeAll();
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;
    std::vector<KeyCombination> getAllKeyCombination() const;

protected:
    /// @brief Get the `autoRepeat` and `callback` values corresponding to the specified key combination.
    /// @note Thread-safe.
    std::pair<bool, std::function<void()>> getValue(const KeyCombination& kc) const;

    /// @brief Indicates the worker thread creation successfully.
    void setSuccessRunning();
    /// @brief Indicates the worker thread creation failed.
    void setFailedRunning(int errorCode);

    // Some interfaces for subclasses to specialize.

    /// @note This function will be executed before the worker thread is created.
    virtual int doBeforeThreadStart();
    /// @note This function will be executed before the end of the worker thread.
    /// @note Specifically, only when this function returns will the semaphore controlling
    /// the thread's exit be changed to enable the thread to exit.
    /// @note In fact, it should always return 'RC_SUCCESS'.
    virtual int doBeforeThreadEnd();
    /// @note The specific working logic of the worker thread.
    /// @attention The `setSuccessRunning` or `setFailedRunning` must be called in the implementation
    /// of this function to indicate whether the work started successfully.
    virtual void work() = 0;
    /// @note The specific logic of register hotkey.
    virtual int registerHotkey(const KeyCombination& kc, bool autoRepeat) = 0;
    /// @note The specific logic of unregister hotkey.
    /// @note In fact, it should always return 'RC_SUCCESS'.
    virtual int unregisterHotkey(const KeyCombination& kc) = 0;

private:
    enum RunningState
    {
        RS_FREE,
        RS_RUNNING,
        RS_TERMINATE
    };

    bool isInWorkerThread() const;

    mutable std::mutex mtx;
    std::condition_variable cvRunningState;
    std::atomic<RunningState> runningState;
    std::atomic<int> createThreadRc;

    std::thread workerThread;
    std::thread::id workerThreadId;

    std::unordered_map<KeyCombination, std::pair<bool, std::function<void()>>> fns;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GHM_PRIVATE_HPP
