#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_BASE_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_BASE_HPP

#include <atomic>   // atomic
#include <chrono>   // chrono
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread

#include "core/base.hpp"
#include "core/return_code.hpp"
#include "keycombination.hpp"

namespace gbhk
{

void GBHK_API sleep(size_t millisecond);

class GBHK_API GlobalHotkeyBase
{
public:
    bool isRunning() const;
    void setIntervalTime(size_t millisecond);

    virtual int start() = 0;
    virtual int end() = 0;
    /// @brief                  Register a global hotkey.
    /// @param keycomb          The key combination to register.
    /// @param callbackFunc     The callback function to be called when the hotkey is triggered.
    /// @return                 The return code of the operation.
    virtual int add(const KeyCombination& keycomb, VoidFunc callbackFunc) = 0;
    /// @brief                  Register a global hotkey.
    /// @param keycomb          The key combination to register.
    /// @param callbackFunc     The callback function to be called when the hotkey is triggered.
    /// @param arg              The argument to be passed to the callback function.
    /// @return                 The return code of the operation.
    virtual int add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg) = 0;
    /// @brief                  Unregister a global hotkey.
    /// @param keycomb          The key combination to unregister.
    /// @return                 The return code of the operation.
    virtual int remove(const KeyCombination& keycomb) = 0;
    /// @brief                  Replace a global hotkey.
    /// @param oldKeycomb       The old key combination to be replaced.
    /// @param newKeycomb       The new key combination to replace with.
    /// @return                 The return code of the operation.
    virtual int replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb) = 0;

protected:
    GlobalHotkeyBase();
    ~GlobalHotkeyBase();
    GlobalHotkeyBase(const GlobalHotkeyBase& other) = delete;
    GlobalHotkeyBase& operator=(const GlobalHotkeyBase& other) = delete;

    void setTimePoint_();
    void waitInterval_();

    void setWorkThreadId_(const std::thread::id& id);
    std::thread::id getWorkThreadId_();

    std::atomic<bool> isRunning_;
    std::atomic<bool> shouldClose_;
    std::thread workThread_;

private:
    std::atomic<size_t> intervalTime_;
    std::chrono::steady_clock::time_point timePoint_;
    std::mutex mtx_;
    std::thread::id workThreadId_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_BASE_HPP
