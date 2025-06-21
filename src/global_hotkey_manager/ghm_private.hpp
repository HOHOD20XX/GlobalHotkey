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
    void setCycleTime(size_t milliseconds);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;
    std::vector<KeyCombination> getAllKeyCombination() const;

protected:
    bool isInWorkThread() const;
    std::pair<bool, std::function<void()>> getValue(const KeyCombination& kc) const;

    virtual int specializedStart();
    virtual int specializedEnd();
    virtual int specializedAdd(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat);
    virtual int specializedRemove(const KeyCombination& kc);
    virtual int specializedRemoveAll();
    virtual int specializedReplace(const KeyCombination& oldKc, const KeyCombination& newKc);
    virtual int specializedSetAutoRepeat(const KeyCombination& kc, bool autoRepeat);

    virtual int doBeforeLoop();
    virtual int doAfterLoop();
    virtual void eachCycleDo() = 0;

private:
    void workLoop();

    mutable std::mutex mtx;
    std::condition_variable cvDoBeforeLoopFinished;
    std::condition_variable cvRunning;
    std::atomic<bool> doBeforeLoopFinished;
    std::atomic<bool> shouldClose;
    std::atomic<bool> running;
    std::atomic<size_t> cycleTime;

    std::thread workThread;
    std::thread::id workThreadId;

    std::unordered_map<KeyCombination, std::pair<bool, std::function<void()>>> fns;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GHM_PRIVATE_HPP
