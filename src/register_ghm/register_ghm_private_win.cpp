#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <global_hotkey/utility.hpp>

namespace gbhk
{

_RegisterGHMPrivateWin::_RegisterGHMPrivateWin() :
    hasTask_(false), taskFinished_(false), taskResult_(RC_SUCCESS), hotkeyIndex_(0)
{}

_RegisterGHMPrivateWin::~_RegisterGHMPrivateWin()
{
    end();
}

int _RegisterGHMPrivateWin::start()
{
    if (isRunning_)
        return RC_SUCCESS;

    isRunning_ = true;
    workThread_ = std::thread([=]()
    {
        workThreadId_ = CUR_TH_ID;
        work_();
        isRunning_ = false;
    });
    workThread_.detach();
    return RC_SUCCESS;
}

int _RegisterGHMPrivateWin::end()
{
    if (!isRunning_)                    return RC_SUCCESS;
    if (CUR_TH_ID == workThreadId_)     return RC_END_GHM_IN_WRONG_THREAD;

    Task tsk;
    tsk.op = Task::END;
    setTask_(tsk);
    int rtn = waitTask_();

    shouldClose_ = true;
    while (isRunning_)
        yield();
    shouldClose_ = false;
    workThreadId_ = std::thread::id();
    voidFuncs_.clear();
    argFuncArgs_.clear();

    return rtn;
}

int _RegisterGHMPrivateWin::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{
    if (!kc.isValid() || func == nullptr)   return RC_INVALID_VALUE;
    if (!isRunning_)                        return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)         return RC_ARR_GHM_IN_WRONG_THREAD;
    if (has(kc))                            return RC_EXIST_SAME_VALUE;

    Task tsk;
    tsk.op = Task::ADD;
    tsk.add.kc = kc;
    tsk.add.autoRepeat = autoRepeat;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        voidFuncs_.insert({ kc, { autoRepeat, func } });
    }

    return rtn;
}

int _RegisterGHMPrivateWin::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{
    if (!kc.isValid() || func == nullptr)   return RC_INVALID_VALUE;
    if (!isRunning_)                        return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)         return RC_ARR_GHM_IN_WRONG_THREAD;
    if (has(kc))                            return RC_EXIST_SAME_VALUE;

    Task tsk;
    tsk.op = Task::ADD;
    tsk.add.kc = kc;
    tsk.add.autoRepeat = autoRepeat;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        argFuncArgs_.insert({ kc, { autoRepeat, { func, arg } } });
    }

    return rtn;
}

int _RegisterGHMPrivateWin::remove(const KeyCombination& kc)
{
    if (!isRunning_)                    return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)     return RC_ARR_GHM_IN_WRONG_THREAD;
    if (!has(kc))                       return RC_NO_SPECIFIED_VALUE;

    Task tsk;
    tsk.op = Task::REMOVE;
    tsk.remove.kc = kc;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        if (voidFuncs_.find(kc) != voidFuncs_.end())
            voidFuncs_.erase(kc);
        else
            argFuncArgs_.erase(kc);
    }

    return rtn;
}

int _RegisterGHMPrivateWin::removeAll()
{
    if (!isRunning_)                    return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)     return RC_ARR_GHM_IN_WRONG_THREAD;

    Task tsk;
    tsk.op = Task::REMOVE_ALL;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        voidFuncs_.clear();
        argFuncArgs_.clear();
    }

    return rtn;
}

int _RegisterGHMPrivateWin::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!newKc.isValid())               return RC_INVALID_VALUE;
    if (!isRunning_)                    return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)     return RC_ARR_GHM_IN_WRONG_THREAD;
    if (oldKc == newKc)                 return RC_NO_CHANGED_VALUE;
    if (!has(oldKc))                    return RC_NO_SPECIFIED_VALUE;
    if (has(newKc))                     return RC_EXIST_SAME_VALUE;

    Task tsk;
    tsk.op = Task::REPLACE;
    tsk.replace.oldKc = oldKc;
    tsk.replace.newKc = newKc;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        if (voidFuncs_.find(oldKc) != voidFuncs_.end())
        {
            auto& pair = voidFuncs_[oldKc];
            voidFuncs_.erase(oldKc);
            voidFuncs_.insert({ newKc, pair });
        }
        else
        {
            auto& pair = argFuncArgs_[oldKc];
            argFuncArgs_.erase(oldKc);
            argFuncArgs_.insert({ newKc, pair });
        }
    }

    return rtn;
}

int _RegisterGHMPrivateWin::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (!isRunning_)                    return RC_ARR_GHM_IN_WRONG_TIME;
    if (CUR_TH_ID == workThreadId_)     return RC_ARR_GHM_IN_WRONG_THREAD;
    if (!has(kc))                       return RC_NO_SPECIFIED_VALUE;
    if (isAutoRepeat(kc) == autoRepeat) return RC_NO_CHANGED_VALUE;

    Task tsk;
    tsk.op = Task::SET_AUTO_REPEAT;
    tsk.setAutoRepeat.kc = kc;
    tsk.setAutoRepeat.autoRepeat = autoRepeat;

    setTask_(tsk);
    int rtn = waitTask_();
    if (rtn == RC_SUCCESS)
    {
        LOCK_MUTEX(mtx_);
        if (voidFuncs_.find(kc) != voidFuncs_.end())
        {
            auto& pair = voidFuncs_[kc];
            pair.first = autoRepeat;
        }
        else
        {
            auto& pair = argFuncArgs_[kc];
            pair.first = autoRepeat;
        }
    }

    return rtn;
}

void _RegisterGHMPrivateWin::work_()
{
    TimedSleeper ts;
    while (!shouldClose_)
    {
        ts.resetStartTime();

        Task tsk = getTask_();
        if (tsk.op != Task::NONE)
        {
            switch (tsk.op)
            {
                case Task::ADD:
                    taskResult_ = add_(tsk.add.kc, tsk.add.autoRepeat);
                    break;
                case Task::REMOVE:
                    taskResult_ = remove_(tsk.remove.kc);
                    break;
                case Task::REMOVE_ALL:
                    taskResult_ = removeAll_();
                    break;
                case Task::REPLACE:
                    taskResult_ = replace_(tsk.replace.oldKc, tsk.replace.newKc);
                    break;
                case Task::SET_AUTO_REPEAT:
                    taskResult_ = setAutoRepeat_(tsk.setAutoRepeat.kc, tsk.setAutoRepeat.autoRepeat);
                    break;
                case Task::END:
                    taskResult_ = end_();
                    break;
                default:
                    break;
            }
            taskFinished_ = true;
        }

        handleEvent_();

        ts.sleepUntilElapsed(cycleTime_);
    }
}

void _RegisterGHMPrivateWin::handleEvent_()
{
    MSG msg = {0};
    while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            int hotkeyId = static_cast<int>(msg.wParam);
            if (hotkeyIdToKc_.find(hotkeyId) != hotkeyIdToKc_.end())
            {
                auto& kc = hotkeyIdToKc_[hotkeyId];
                if (voidFuncs_.find(kc) != voidFuncs_.end())
                {
                    auto& voidFunc = voidFuncs_[kc].second;
                    if (voidFunc)
                        voidFunc();
                }
                else
                {
                    auto& argFuncArg = argFuncArgs_[kc].second;
                    auto& func = argFuncArg.first;
                    auto& arg = argFuncArg.second;
                    if (func)
                        func(arg);
                }
            }
        }

        ::TranslateMessage(&msg);
        ::DispatchMessageA(&msg);
    }
}

int _RegisterGHMPrivateWin::end_()
{
    return removeAll_();
}

int _RegisterGHMPrivateWin::add_(const KeyCombination& kc, bool autoRepeat)
{
    int nativeModifiers = kc.nativeModifiers();
    nativeModifiers |= (!autoRepeat ? MOD_NOREPEAT : 0);
    if (::RegisterHotKey(NULL, hotkeyIndex_, nativeModifiers, kc.nativeKey()))
    {
        hotkeyIdToKc_.insert({ hotkeyIndex_, kc });
        kcToHotkeyId_.insert({ kc, hotkeyIndex_ });
        hotkeyIndex_++;
        return RC_SUCCESS;
    }
    else
    {
        return ::GetLastError();
    }
}

int _RegisterGHMPrivateWin::remove_(const KeyCombination& kc)
{
    int hotkeyId = kcToHotkeyId_[kc];
    if (::UnregisterHotKey(NULL, hotkeyId) != 0)
    {
        hotkeyIdToKc_.erase(hotkeyId);
        kcToHotkeyId_.erase(kc);
        return RC_SUCCESS;
    }
    else
    {
        return ::GetLastError();
    }
}

int _RegisterGHMPrivateWin::removeAll_()
{
    int ec = RC_SUCCESS;
    for (const auto& var : hotkeyIdToKc_)
    {
        int hotkeyId = var.first;
        if (::UnregisterHotKey(NULL, hotkeyId) == 0)
            ec = ::GetLastError();
    }

    hotkeyIndex_ = 0;
    hotkeyIdToKc_.clear();
    kcToHotkeyId_.clear();

    return ec;
}

int _RegisterGHMPrivateWin::replace_(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    int hotkeyId = kcToHotkeyId_[oldKc];
    bool autoRepeat = isAutoRepeat(oldKc);
    if (::UnregisterHotKey(NULL, hotkeyId) != 0)
    {
        hotkeyIdToKc_.erase(hotkeyId);
        kcToHotkeyId_.erase(oldKc);

        int nativeModifiers = newKc.nativeModifiers();
        nativeModifiers |= (!autoRepeat ? MOD_NOREPEAT : 0);
        if (::RegisterHotKey(NULL, hotkeyId, nativeModifiers, newKc.nativeKey()) != 0)
        {
            hotkeyIdToKc_.insert({ hotkeyId, newKc });
            kcToHotkeyId_.insert({ newKc, hotkeyId });
            return RC_SUCCESS;
        }
        else
        {
            return ::GetLastError();
        }
    }
    else
    {
        return ::GetLastError();
    }
}

int _RegisterGHMPrivateWin::setAutoRepeat_(const KeyCombination& kc, bool autoRepeat)
{
    int hotkeyId = kcToHotkeyId_[kc];
    if (::UnregisterHotKey(NULL, hotkeyId) != 0)
    {
        hotkeyIdToKc_.erase(hotkeyId);
        kcToHotkeyId_.erase(kc);

        int nativeModifiers = kc.nativeModifiers();
        nativeModifiers |= (!autoRepeat ? MOD_NOREPEAT : 0);
        if (::RegisterHotKey(NULL, hotkeyId, nativeModifiers, kc.nativeKey()) != 0)
        {
            hotkeyIdToKc_.insert({ hotkeyId, kc });
            kcToHotkeyId_.insert({ kc, hotkeyId });
            return RC_SUCCESS;
        }
        else
        {
            return ::GetLastError();
        }
    }
    else
    {
        return ::GetLastError();
    }
}

void _RegisterGHMPrivateWin::setTask_(const Task& task)
{
    LOCK_MUTEX(mtx_);
    task_ = task;
    hasTask_ = true;
    taskFinished_ = false;
}

_RegisterGHMPrivateWin::Task _RegisterGHMPrivateWin::getTask_()
{
    if (hasTask_)
    {
        LOCK_MUTEX(mtx_);
        hasTask_ = false;
        return task_;
    }
    else
    {
        return Task();
    }
}

int _RegisterGHMPrivateWin::waitTask_() const
{
    while (!taskFinished_)
        yield();
    return taskResult_;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
