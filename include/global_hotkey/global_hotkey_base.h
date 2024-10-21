#ifndef GLOBAL_HOTKEY_BASE_H
#define GLOBAL_HOTKEY_BASE_H

#include <atomic>
#include <mutex>
#include <thread>

#include "core/core.h"
#include "keycombination.h"

namespace gbhk
{

class GlobalHotkeyBase
{
public:
    void setDelay(uint ms);

    GBHK_NODISCARD virtual uint start() = 0;
    GBHK_NODISCARD virtual uint end() = 0;
    GBHK_NODISCARD virtual uint add(const KeyCombination &keycomb, VoidFunc func) = 0;
    GBHK_NODISCARD virtual uint add(const KeyCombination &keycomb, ArgFunc func, Arg arg) = 0;
    GBHK_NODISCARD virtual uint remove(const KeyCombination &keycomb) = 0;
    GBHK_NODISCARD virtual uint replace(const KeyCombination &oldKeycomb,
                                        const KeyCombination &newKeycomb) = 0;

protected:
    GlobalHotkeyBase();
    ~GlobalHotkeyBase();
    GlobalHotkeyBase(const GlobalHotkeyBase &other) = delete;
    GlobalHotkeyBase &operator=(const GlobalHotkeyBase &other) = delete;

    void sleep_(uint ms);
    void setWorkThreadId_(const std::thread::id &id);
    std::thread::id getWorkThreadId_();

    std::atomic<bool> isRunning_;
    std::atomic<bool> shouldClose_;
    std::atomic<uint> delay_;
    std::thread workThread_;

private:
    std::thread::id workThreadId_;
    std::mutex mtx_;
};

}

#endif // !GLOBAL_HOTKEY_BASE_H
