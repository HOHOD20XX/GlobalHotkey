#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP

#include <atomic>   // atomic
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread, thread::id

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

    virtual int start();
    virtual int end();
    virtual int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat = false);
    virtual int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat = false);
    virtual int remove(const KeyCombination& kc);
    virtual int removeAll();
    virtual int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    virtual int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
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
    Map<KeyCombination, Pair<bool, VoidFunc>, KeyCombination::Hash> voidFuncs_;
    Map<KeyCombination, Pair<bool, ArgFuncArg>, KeyCombination::Hash> argFuncArgs_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_PRIVATE_HPP
