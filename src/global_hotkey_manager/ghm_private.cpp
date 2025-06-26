#include "ghm_private.hpp"

#include <global_hotkey/base.hpp>
#include <global_hotkey/return_code.hpp>

namespace gbhk
{

_GHMPrivate::_GHMPrivate() :
    runningState(RS_FREE), createThreadRc(RC_NOT_USED), workerThreadId(std::thread::id())
{}

_GHMPrivate::~_GHMPrivate() = default;

int _GHMPrivate::start()
{
    if (isRunning())            return RC_SUCCESS;

    int rc = doBeforeThreadStart();
    if (rc != RC_SUCCESS)
        return rc;

    createThreadRc = RC_NOT_USED;
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

    return createThreadRc;
}

int _GHMPrivate::end()
{
    if (!isRunning())           return RC_SUCCESS;
    if (isInWorkerThread())     return RC_END_GHM_IN_WRONG_THREAD;

    int rc = removeAll();
    rc = doBeforeThreadEnd();

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunningState.wait(lock, [this]() { return runningState != RS_RUNNING; });
    lock.unlock();

    workerThreadId = std::thread::id();
    fns.clear();

    return rc;
}

int _GHMPrivate::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{
    if (!isRunning())           return RC_CHANGE_GH_IN_WRONG_TIME;
    if (isInWorkerThread())     return RC_CHANGE_GH_IN_WRONG_THREAD;
    if (!kc.isValid() || !fn)   return RC_INVALID_VALUE;
    if (has(kc))                return RC_EXIST_SAME_VALUE;

    int rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[kc] = {autoRepeat, fn};

    return RC_SUCCESS;
}

int _GHMPrivate::remove(const KeyCombination& kc)
{
    if (!isRunning())           return RC_CHANGE_GH_IN_WRONG_TIME;
    if (isInWorkerThread())     return RC_CHANGE_GH_IN_WRONG_THREAD;
    if (!has(kc))               return RC_NO_SPECIFIED_VALUE;

    int rc = unregisterHotkey(kc);

    std::lock_guard<std::mutex> lock(mtx);
    fns.erase(kc);

    return rc;
}

int _GHMPrivate::removeAll()
{
    if (!isRunning())           return RC_CHANGE_GH_IN_WRONG_TIME;
    if (isInWorkerThread())     return RC_CHANGE_GH_IN_WRONG_THREAD;

    int rc = RC_SUCCESS;
    for (const auto& var : fns)
        rc = unregisterHotkey(var.first);

    std::lock_guard<std::mutex> lock(mtx);
    fns.clear();

    return rc;
}

int _GHMPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!isRunning())           return RC_CHANGE_GH_IN_WRONG_TIME;
    if (isInWorkerThread())     return RC_CHANGE_GH_IN_WRONG_THREAD;
    if (!newKc.isValid())       return RC_INVALID_VALUE;
    if (!has(oldKc))            return RC_NO_SPECIFIED_VALUE;
    if (has(newKc))             return RC_EXIST_SAME_VALUE;

    if (newKc == oldKc)
        return RC_SUCCESS;

    auto value = getValue(oldKc);
    int rc = unregisterHotkey(oldKc);
    mtx.lock();
    fns.erase(oldKc);
    mtx.unlock();
    rc = registerHotkey(newKc, value.first);
    // No Error Recall! That is if register the new KC is failed, the old KC still be unregister.
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[newKc] = value;

    return RC_SUCCESS;
}

int _GHMPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!isRunning())           return RC_CHANGE_GH_IN_WRONG_TIME;
    if (isInWorkerThread())     return RC_CHANGE_GH_IN_WRONG_THREAD;
    if (!has(kc))               return RC_NO_SPECIFIED_VALUE;


    auto value = getValue(kc);
    if (value.first == autoRepeat)
        return RC_SUCCESS;

    int rc = unregisterHotkey(kc);
    mtx.lock();
    fns.erase(kc);
    mtx.unlock();

    rc = registerHotkey(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    value.first = autoRepeat;
    std::lock_guard<std::mutex> lock(mtx);
    fns[kc] = value;

    return RC_SUCCESS;
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

std::vector<KeyCombination> _GHMPrivate::getAllKeyCombination() const
{
    std::vector<KeyCombination> rslt;
    for (const auto& var : fns)
        rslt.emplace_back(var.first);
    return rslt;
}

std::pair<bool, std::function<void()>> _GHMPrivate::getValue(const KeyCombination& kc) const
{
    if (!kc.isValid() || !has(kc))
        return std::pair<bool, std::function<void()>>();

    std::lock_guard<std::mutex> lock(mtx);
    return fns.at(kc);
}

void _GHMPrivate::setSuccessRunning()
{
    createThreadRc = RC_SUCCESS;
    runningState = RS_RUNNING;
    cvRunningState.notify_one();
}

void _GHMPrivate::setFailedRunning(int errorCode)
{
    createThreadRc = errorCode;
    runningState = RS_TERMINATE;
    cvRunningState.notify_one();
}

int _GHMPrivate::doBeforeThreadStart()
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
