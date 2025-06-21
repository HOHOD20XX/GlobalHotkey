#include "ghm_private.hpp"

#include <global_hotkey/base.hpp>
#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

namespace gbhk
{

_GHMPrivate::_GHMPrivate() :
    doBeforeLoopFinished(false), shouldClose(false), running(false), cycleTime(_DEFAULT_CYCLE_TIME)
{}

_GHMPrivate::~_GHMPrivate() = default;

int _GHMPrivate::start()
{
    if (running)                return RC_SUCCESS;

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

        workThreadId = std::this_thread::get_id();
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

int _GHMPrivate::end()
{
    if (!running)               return RC_SUCCESS;
    if (isInWorkThread())       return RC_END_GHM_IN_WRONG_THREAD;

    int rc = specializedEnd();

    shouldClose = true;
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRunning.wait(lock, [this]() { return !running; });
    lock.unlock();
    shouldClose = false;

    workThreadId = std::thread::id();
    fns.clear();

    return rc;
}

int _GHMPrivate::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{
    if (!running)               return RC_CHANGE_GH_IN_WRONG_TIME;
    if (!kc.isValid() || !fn)   return RC_INVALID_VALUE;
    if (has(kc))                return RC_EXIST_SAME_VALUE;

    int rc = specializedAdd(kc, fn, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[kc] = {autoRepeat, fn};

    return RC_SUCCESS;
}

int _GHMPrivate::remove(const KeyCombination& kc)
{
    if (!running)               return RC_CHANGE_GH_IN_WRONG_TIME;
    if (!has(kc))               return RC_NO_SPECIFIED_VALUE;

    int rc = specializedRemove(kc);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns.erase(kc);

    return RC_SUCCESS;
}

int _GHMPrivate::removeAll()
{
    if (!running)               return RC_CHANGE_GH_IN_WRONG_TIME;

    int rc = specializedRemoveAll();
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns.clear();

    return RC_SUCCESS;
}

int _GHMPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!running)               return RC_CHANGE_GH_IN_WRONG_TIME;
    if (!newKc.isValid())       return RC_INVALID_VALUE;
    if (!has(oldKc))            return RC_NO_SPECIFIED_VALUE;
    if (has(newKc))             return RC_EXIST_SAME_VALUE;

    if (newKc == oldKc)
        return RC_SUCCESS;

    int rc = specializedReplace(oldKc, newKc);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[newKc] = std::move(fns[oldKc]);
    fns.erase(oldKc);

    return RC_SUCCESS;
}

int _GHMPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!running)               return RC_CHANGE_GH_IN_WRONG_TIME;
    if (!has(kc))               return RC_NO_SPECIFIED_VALUE;

    int rc = specializedSetAutoRepeat(kc, autoRepeat);
    if (rc != RC_SUCCESS)
        return rc;

    std::lock_guard<std::mutex> lock(mtx);
    fns[kc].first = autoRepeat;

    return RC_SUCCESS;
}

void _GHMPrivate::setCycleTime(size_t milliseconds)
{
    cycleTime = milliseconds;
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
    return running;
}

std::vector<KeyCombination> _GHMPrivate::getAllKeyCombination() const
{
    std::vector<KeyCombination> rslt;
    for (const auto& var : fns)
        rslt.emplace_back(var.first);
    return rslt;
}

void _GHMPrivate::workLoop()
{
    TimedSleeper ts;
    while (!shouldClose)
    {
        ts.resetStartTime();
        eachCycleDo();
        ts.sleepUntilElapsed(cycleTime);
    }
}

bool _GHMPrivate::isInWorkThread() const
{
    return std::this_thread::get_id() == workThreadId;
}

std::pair<bool, std::function<void()>> _GHMPrivate::getValue(const KeyCombination& kc) const
{
    if (!kc.isValid() || !has(kc))
        return std::pair<bool, std::function<void()>>();

    std::lock_guard<std::mutex> lock(mtx);
    return fns.at(kc);
}

int _GHMPrivate::specializedStart()
{ return RC_SUCCESS; }

int _GHMPrivate::specializedEnd()
{ return RC_SUCCESS; }

int _GHMPrivate::specializedAdd
(const KeyCombination &kc, const std::function<void()> &fn, bool autoRepeat)
{ return RC_SUCCESS; }

int _GHMPrivate::specializedRemove(const KeyCombination &kc)
{ return RC_SUCCESS; }

int _GHMPrivate::specializedRemoveAll()
{ return RC_SUCCESS; }

int _GHMPrivate::specializedReplace
(const KeyCombination &oldKc, const KeyCombination &newKc)
{ return RC_SUCCESS; }

int _GHMPrivate::specializedSetAutoRepeat
(const KeyCombination &kc, bool autoRepeat)
{ return RC_SUCCESS; }

int _GHMPrivate::doBeforeLoop()
{ return RC_SUCCESS; }

int _GHMPrivate::doAfterLoop()
{ return RC_SUCCESS; }

} // namespace gbhk
