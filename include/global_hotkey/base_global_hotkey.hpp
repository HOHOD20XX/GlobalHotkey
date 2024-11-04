#ifndef GLOBAL_HOTKEY_BASE_GLOBAL_HOTKEY_HPP
#define GLOBAL_HOTKEY_BASE_GLOBAL_HOTKEY_HPP

#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>

#include "core/core.hpp"
#include "keycombination.hpp"

namespace gbhk
{

void sleep(ullong millisecond);

class GlobalHotkeyBase
{
public:
    bool isRunning() const;
    void setIntervalTime(ullong millisecond);

    GBHK_NODISCARD virtual uint start() = 0;
    GBHK_NODISCARD virtual uint end() = 0;
    GBHK_NODISCARD virtual uint add(const KeyCombination& keycomb, VoidFunc callbackFunc) = 0;
    GBHK_NODISCARD virtual uint add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg) = 0;
    GBHK_NODISCARD virtual uint remove(const KeyCombination& keycomb) = 0;
    GBHK_NODISCARD virtual uint replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb) = 0;

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
    std::atomic<ullong> intervalTime_;
    std::chrono::steady_clock::time_point timePoint_;
    std::mutex mtx_;
    std::thread::id workThreadId_;
};

}

#endif // !GLOBAL_HOTKEY_BASE_GLOBAL_HOTKEY_HPP
