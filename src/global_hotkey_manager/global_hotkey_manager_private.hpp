#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP

#include <atomic>           // atomic
#include <functional>       // function
#include <mutex>            // mutex, lock_guard
#include <thread>           // thread, thread::id
#include <unordered_map>    // unordered_map

#include <global_hotkey/base.hpp>
#include <global_hotkey/return_code.hpp>
#include <global_hotkey/key_combination.hpp>

#define LOCK_MUTEX(mtx) std::lock_guard<std::mutex> lock(mtx)
#define CUR_TH_ID std::this_thread::get_id()

namespace gbhk
{

class _GlobalHotkeyManagerPrivate
{
public:
    _GlobalHotkeyManagerPrivate();
    virtual ~_GlobalHotkeyManagerPrivate();

    virtual int start() = 0;
    virtual int end() = 0;
    virtual int add(const KeyCombination& kc, const std::function<void()>& func, bool autoRepeat = false) = 0;
    virtual int add(const KeyCombination& kc, std::function<void()>&& func, bool autoRepeat = false) = 0;
    virtual int remove(const KeyCombination& kc) = 0;
    virtual int removeAll() = 0;
    virtual int replace(const KeyCombination& oldKc, const KeyCombination& newKc) = 0;
    virtual int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) = 0;
    int setCycleTime(size_t milliseconds);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;

protected:
    mutable std::mutex mtx_;
    std::atomic<bool> isRunning_;
    std::atomic<bool> shouldClose_;
    std::atomic<size_t> cycleTime_;

    std::thread workThread_;
    std::thread::id workThreadId_;

    std::unordered_map<KeyCombination, std::pair<bool, std::function<void()>>> funcs_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP
