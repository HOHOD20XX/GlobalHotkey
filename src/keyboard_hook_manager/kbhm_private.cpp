#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

namespace kbhook
{

std::mutex _KBHMPrivate::mtx;
std::unordered_map<_KBHMPrivate::Combination, std::function<void ()>> _KBHMPrivate::fns;
std::function<void (int)> _KBHMPrivate::keyPressedCallback;
std::function<void (int)> _KBHMPrivate::keyReleasedCallback;

_KBHMPrivate::_KBHMPrivate() :
    runningState(RS_FREE),
    runningRc(0),
    shouldClose(false)
{}

_KBHMPrivate::~_KBHMPrivate() = default;

int _KBHMPrivate::run()
{
    if (isRunning())    return RC_SUCCESS;

    int rc = doBeforeThreadRun();
    if (rc != RC_SUCCESS)
        return rc;

    workerThread = std::thread([this]() {
        work();
        runningState = RS_FREE;
        cvRunningState.notify_one();
    });
    workerThread.detach();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState.wait(lock, [this]() { return runningState != RS_FREE; });
    lock.unlock();

    if (runningState == RS_TERMINATE)
        runningState = RS_FREE;

    return runningRc;
}

int _KBHMPrivate::end()
{
    if (!isRunning())   return RC_SUCCESS;

    {
        std::lock_guard<std::mutex> lock(mtx);
        fns.clear();
        keyPressedCallback = nullptr;
        keyReleasedCallback = nullptr;
    }
    int rc = doBeforeThreadEnd();

    shouldClose = true;
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState.wait(lock, [this]() { return runningState != RS_RUNNING; });
    lock.unlock();

    return rc;
}

int _KBHMPrivate::addKeyListener(int nativeKey, KeyState state, const std::function<void ()>& fn)
{
    std::lock_guard<std::mutex> lock(mtx);
    fns[{nativeKey, state}] = fn;
    return RC_SUCCESS;
}

int _KBHMPrivate::removeKeyListener(int nativeKey, KeyState state)
{
    std::lock_guard<std::mutex> lock(mtx);
    fns.erase({nativeKey, state});
    return RC_SUCCESS;
}

int _KBHMPrivate::removeAllKeyListener()
{
    std::lock_guard<std::mutex> lock(mtx);
    fns.clear();
    return RC_SUCCESS;
}

int _KBHMPrivate::setKeyPressedCallback(const std::function<void (int)>& fn)
{
    std::lock_guard<std::mutex> lock(mtx);
    keyPressedCallback = fn;
    return RC_SUCCESS;
}

int _KBHMPrivate::setKeyReleasedCallback(const std::function<void (int)>& fn)
{
    std::lock_guard<std::mutex> lock(mtx);
    keyReleasedCallback = fn;
    return RC_SUCCESS;
}

int _KBHMPrivate::unsetKeyPressedCallback()
{
    std::lock_guard<std::mutex> lock(mtx);
    keyPressedCallback = nullptr;
    return RC_SUCCESS;
}

int _KBHMPrivate::unsetKeyReleasedCallback()
{
    std::lock_guard<std::mutex> lock(mtx);
    keyReleasedCallback = nullptr;
    return RC_SUCCESS;
}

bool _KBHMPrivate::hasKeyListener(int nativeKey, KeyState state)
{
    std::lock_guard<std::mutex> lock(mtx);
    return fns.find({nativeKey, state}) != fns.end();
}

bool _KBHMPrivate::isRunning() const
{
    return runningState == RS_RUNNING;
}

std::function<void ()> _KBHMPrivate::getKeyListenerCallback(int nativeKey, KeyState state)
{
    std::lock_guard<std::mutex> lock(mtx);
    const auto& it = fns.find({nativeKey, state});
    if (it == fns.end())
        return std::function<void ()>();
    return it->second;
}

std::function<void (int)> _KBHMPrivate::getKeyPressedCallback()
{
    std::lock_guard<std::mutex> lock(mtx);
    return keyPressedCallback;
}

std::function<void (int)> _KBHMPrivate::getKeyReleasedCallback()
{
    std::lock_guard<std::mutex> lock(mtx);
    return keyReleasedCallback;
}

void _KBHMPrivate::setRunSuccess()
{
    runningRc = RC_SUCCESS;
    runningState = RS_RUNNING;
    cvRunningState.notify_one();
}

void _KBHMPrivate::setRunFail(int errorCode)
{
    runningRc = errorCode;
    runningState = RS_TERMINATE;
    cvRunningState.notify_one();
}

int _KBHMPrivate::doBeforeThreadRun() { return RC_SUCCESS; }

int _KBHMPrivate::doBeforeThreadEnd() { return RC_SUCCESS; }

bool _KBHMPrivate::isValidValue(int nativeKey, KeyState state)
{
    return nativeKey != 0 && state != KS_NONE;
}

} // namespace kbhook

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
