#include "ghm_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

_GHMPrivate::_GHMPrivate() :
    runningState(RS_FREE),
    runningRc(0),
    workerThreadId(std::thread::id())
{}

_GHMPrivate::~_GHMPrivate() = default;

int _GHMPrivate::run()
{
    if (isRunning())            return RC_SUCCESS;

    int rc = doBeforeThreadRun();
    if (rc != RC_SUCCESS)
        return rc;

    workerThread = std::thread([this]() {
        workerThreadId = std::this_thread::get_id();
        work();
        runningState = RS_FREE;
        cvRunningState.notify_one();
    });
    workerThread.detach();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState.wait(lock, [this]() { return runningState != RS_FREE; });

    if (runningState == RS_TERMINATE)
    {
        workerThreadId = std::this_thread::get_id();
        runningState = RS_FREE;
    }

    return runningRc;
}

int _GHMPrivate::end()
{
    if (!isRunning())           return RC_SUCCESS;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = removeAll();
    rc = doBeforeThreadEnd();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState.wait(lock, [this]() { return runningState != RS_RUNNING; });

    return rc;
}

int _GHMPrivate::add(const KeyCombination& kc, const std::function<void ()>& fn, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[kc] = {autoRepeat, fn};

    return rc;
}

int _GHMPrivate::remove(const KeyCombination& kc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = unregisterHotkey(kc);

    std::lock_guard<std::mutex> lock(mtx);
    fns.erase(kc);

    return rc;
}

int _GHMPrivate::removeAll()
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    int rc = RC_SUCCESS;
    for (const auto& var : fns)
        rc = unregisterHotkey(var.first);

    std::lock_guard<std::mutex> lock(mtx);
    fns.clear();

    return rc;
}

int _GHMPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;
    if (newKc == oldKc)         return RC_SUCCESS;

    auto value = getPairValue(oldKc);
    int rc = unregisterHotkey(oldKc);
    mtx.lock();
    fns.erase(oldKc);
    mtx.unlock();
    rc = registerHotkey(newKc, value.first);
    // No Error Rollback! That is if register the new KC is failed, the old KC still be unregister.
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[newKc] = value;

    return rc;
}

int _GHMPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!isRunning())           return RC_BAD_TIMING;
    if (isInWorkerThread())     return RC_BAD_THREAD;

    auto value = getPairValue(kc);
    if (value.first == autoRepeat)
        return RC_SUCCESS;
    value.first = autoRepeat;

    int rc = unregisterHotkey(kc);
    mtx.lock();
    fns.erase(kc);
    mtx.unlock();
    rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[kc] = value;

    return rc;
}

bool _GHMPrivate::has(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx);
    return fns.find(kc) != fns.end();
}

bool _GHMPrivate::isAutoRepeat(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx);
    if (fns.find(kc) != fns.end())
        return fns.at(kc).first;
    return false;
}

bool _GHMPrivate::isRunning() const
{
    return runningState == RS_RUNNING;
}

std::vector<KeyCombination> _GHMPrivate::getAllHotkeys() const
{
    std::vector<KeyCombination> rslt;
    for (const auto& var : fns)
        rslt.emplace_back(var.first);
    return rslt;
}

std::pair<bool, std::function<void ()>> _GHMPrivate::getPairValue(const KeyCombination& kc) const
{
    std::lock_guard<std::mutex> lock(mtx);
    const auto& it = fns.find(kc);
    if (it == fns.end())
        return std::pair<bool, std::function<void ()>>();
    return it->second;
}

void _GHMPrivate::setRunSuccess()
{
    runningRc = RC_SUCCESS;
    runningState = RS_RUNNING;
    cvRunningState.notify_one();
}

void _GHMPrivate::setRunFail(int errorCode)
{
    runningRc = errorCode;
    runningState = RS_TERMINATE;
    cvRunningState.notify_one();
}

int _GHMPrivate::doBeforeThreadRun()
{ return RC_SUCCESS; }

int _GHMPrivate::doBeforeThreadEnd()
{ return RC_SUCCESS; }

int _GHMPrivate::registerHotkey(const KeyCombination &kc, bool autoRepeat)
{ return RC_SUCCESS; }

int _GHMPrivate::unregisterHotkey(const KeyCombination &kc)
{ return RC_SUCCESS; }

bool _GHMPrivate::isInWorkerThread() const
{
  return std::this_thread::get_id() == workerThreadId;
}

} // namespace gbhk
