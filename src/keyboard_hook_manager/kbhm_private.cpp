#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

namespace gbhk
{

namespace kbhook
{

std::mutex _KBHMPrivate::mtx;
std::unordered_map<_KBHMPrivate::Combine, std::function<void()>> _KBHMPrivate::fns;
void (*_KBHMPrivate::keyPressedCallback)(int)    = nullptr;
void (*_KBHMPrivate::keyReleasedCallback)(int)   = nullptr;

_KBHMPrivate::_KBHMPrivate() :
    doBeforeLoopFinished(false), shouldClose(false), running(false), cycleTime(_DEFAULT_KEYBOARD_HOOK_CYCLE_TIME)
{}

_KBHMPrivate::~_KBHMPrivate() = default;

int _KBHMPrivate::start()
{
    if (running)                            return RC_SUCCESS;

    int rc = specializedStart();
    if (rc != RC_SUCCESS)
        return rc;

    running = true;
    workThread = std::thread([this, &rc]() {
        rc = doBeforeLoop();
        doBeforeLoopFinished = true;
        cvDoBeforeLoopFinished.notify_all();
        if (rc != RC_SUCCESS)
        {
            running = false;
            return;
        }

        workLoop();

        int rc2 = doAfterLoop();
        running = false;
        cvRunning.notify_all();
    });
    workThread.detach();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvDoBeforeLoopFinished.wait(lock, [this]() { return doBeforeLoopFinished.load(); });
    lock.unlock();
    doBeforeLoopFinished = false;

    return rc;
}

int _KBHMPrivate::end()
{
    if (!running)                           return RC_SUCCESS;

    shouldClose = true;
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunning.wait(lock, [this]() { return !running; });
    lock.unlock();
    shouldClose = false;

    // Always is RC_SUCCESS
    int rc = specializedEnd();

    fns.clear();

    return rc;
}

int _KBHMPrivate::addKeyListener(int nativeKey, KeyState state, const std::function<void()>& fn)
{
    if (nativeKey == 0 || !fn)              return RC_INVALID_VALUE;
    if (hasKeyListener(nativeKey, state))   return RC_EXIST_SAME_VALUE;

    std::lock_guard<std::mutex> lock(mtx);
    Combine combine(nativeKey, state);
    fns[combine] = fn;

    return RC_SUCCESS;
}

int _KBHMPrivate::removeKeyListener(int nativeKey, KeyState state)
{
    if (!hasKeyListener(nativeKey, state))  return RC_NO_SPECIFIED_VALUE;

    std::lock_guard<std::mutex> lock(mtx);
    Combine combine(nativeKey, state);
    fns.erase(combine);

    return RC_SUCCESS;
}

int _KBHMPrivate::removeAllKeyListener()
{
    std::lock_guard<std::mutex> lock(mtx);
    fns.clear();
    return RC_SUCCESS;
}

int _KBHMPrivate::setKeyPressedEvent(void (*fn)(int))
{
    if (fn == nullptr)                      return RC_INVALID_VALUE;

    std::lock_guard<std::mutex> lock(mtx);
    keyPressedCallback = fn;

    return RC_SUCCESS;
}

int _KBHMPrivate::setKeyReleasedEvent(void (*fn)(int))
{
    if (fn == nullptr)                      return RC_INVALID_VALUE;

    std::lock_guard<std::mutex> lock(mtx);
    keyReleasedCallback = fn;

    return RC_SUCCESS;
}

int _KBHMPrivate::unsetKeyPressedEvent()
{
    std::lock_guard<std::mutex> lock(mtx);
    keyPressedCallback = nullptr;
    return RC_SUCCESS;
}

int _KBHMPrivate::unsetKeyReleasedEvent()
{
    std::lock_guard<std::mutex> lock(mtx);
    keyReleasedCallback = nullptr;
    return RC_SUCCESS;
}

void _KBHMPrivate::setCycleTime(size_t milliseconds)
{
    cycleTime = milliseconds;
}

bool _KBHMPrivate::hasKeyListener(int nativeKey, KeyState state) const
{
    std::lock_guard<std::mutex> lock(mtx);
    Combine combine(nativeKey, state);
    return fns.find(combine) != fns.end();
}

bool _KBHMPrivate::isRunning() const
{
    return running;
}

int _KBHMPrivate::specializedStart() { return RC_SUCCESS; }

int _KBHMPrivate::specializedEnd() { return RC_SUCCESS; }

int _KBHMPrivate::doBeforeLoop() { return RC_SUCCESS; }

int _KBHMPrivate::doAfterLoop() { return RC_SUCCESS; }

void _KBHMPrivate::workLoop()
{
    TimedSleeper ts;
    while (!shouldClose)
    {
        ts.resetStartTime();
        eachCycleDo();
        ts.sleepUntilElapsed(cycleTime);
    }
}

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
