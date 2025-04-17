#include "global_hotkey/register_global_hotkey.hpp"

#ifndef GLOBAL_HOTKEY_NOREGISTER

#if defined(GLOBAL_HOTKEY_WIN)
    #include <Windows.h>
#elif defined(GLOBAL_HOTKEY_MAC)
    // TODO
#elif defined(GLOBAL_HOTKEY_LINUX)
    // TODO
#endif

namespace gbhk
{

RegGlobalHotkey::RegGlobalHotkey() :
    hasTask_(false), taskIsFinished_(false), taskResult_(0), keyId_(0)
{}

RegGlobalHotkey::~RegGlobalHotkey()
{
    int rslt = end();
}

RegGlobalHotkey& RegGlobalHotkey::getInstance()
{
    static RegGlobalHotkey instance;
    return instance;
}

GBHK_NODISCARD int RegGlobalHotkey::start()
{
    // If the thread is running do nothing.
    if (isRunning_)
        return RC_UNTIMELY_CALL;

    isRunning_ = true;
    workThread_ = std::thread([&]()
    {
        // Get the current thread id and set the #workThreadId_ value.
        setWorkThreadId_(std::this_thread::get_id());

        // The loop of the work thread.
        while (!shouldClose_)
        {
            setTimePoint_();

            // Process possible task first in each loop.
            Task tsk = getTask_();
            // If get a valid task, process this task.
            if (tsk.opc != Task::NONE)
            {
                switch (tsk.opc)
                {
                    case Task::ADD:
                        taskResult_ = add_(tsk.add.keycomb);
                        break;
                    case Task::REMOVE:
                        taskResult_ = remove_(tsk.remove.keycomb);
                        break;
                    case Task::REPLACE:
                        taskResult_ = replace_(tsk.replace.oldKeycomb, tsk.replace.newKeycomb);
                        break;
                    case Task::END:
                        taskResult_ = end_();
                        break;
                    default:
                        break;
                }
                // Set the flag to true indicate the task is be process finished.
                taskIsFinished_ = true;
            }

            work_();

            waitInterval_();
        }

        // Reset the work thread id.
        setWorkThreadId_(std::thread::id());

        isRunning_ = false;
    });
    workThread_.detach();

    return RC_SUCCESS;
}

GBHK_NODISCARD int RegGlobalHotkey::end()
{
    // If the thread is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    Task tsk;
    tsk.opc = Task::END;
    setTask_(tsk);
    int rslt = waitTaskFinished_();

    // Set this flag to true to exit thread.
    shouldClose_ = true;
    // Wait the thread exits.
    while (isRunning_)
        sleep(1);
    // Reset this flag to default state.
    shouldClose_ = false;

    // Clear members.
    // Because the thread is exited, so not has thread safe problem.
    voidFuncs_.clear();
    argFuncArgs_.clear();

    return rslt;
}

GBHK_NODISCARD int RegGlobalHotkey::add(const KeyCombination& keycomb, VoidFunc callbackFunc)
{
    // If the thread is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    if (!keycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    // If the key combination already exists do nothing.
    if (getVoidFunc_(keycomb) || getArgFuncArg_(keycomb).first)
        return RC_ALREADY_EXISTED;

    Task tsk;
    tsk.opc = Task::ADD;
    tsk.add.keycomb = keycomb;

    setTask_(tsk);
    int rslt = waitTaskFinished_();

    if (rslt == RC_SUCCESS)
        addFunc_(keycomb, callbackFunc);

    return rslt;
}

GBHK_NODISCARD int RegGlobalHotkey::add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg)
{
    // If the thread is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    if (!keycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    // If the key combination already exists do nothing.
    if (getVoidFunc_(keycomb) || getArgFuncArg_(keycomb).first)
        return RC_ALREADY_EXISTED;

    Task tsk;
    tsk.opc = Task::ADD;
    tsk.add.keycomb = keycomb;

    setTask_(tsk);
    int rslt = waitTaskFinished_();

    if (rslt == RC_SUCCESS)
        addFunc_(keycomb, callbackFunc, arg);

    return rslt;
}

GBHK_NODISCARD int RegGlobalHotkey::remove(const KeyCombination& keycomb)
{
    // If the thread is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    // If the key combination is not exists do nothing.
    if (getVoidFunc_(keycomb) == nullptr && getArgFuncArg_(keycomb).first == nullptr)
        return RC_NOT_FIND;

    Task tsk;
    tsk.opc = Task::REMOVE;
    tsk.remove.keycomb = keycomb;

    setTask_(tsk);
    int rslt = waitTaskFinished_();

    if (rslt == RC_SUCCESS)
        removeFunc_(keycomb);

    return rslt;
}

GBHK_NODISCARD int RegGlobalHotkey::replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb)
{
    // If the thread is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    if (!newKeycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    // If the old key combination equal to new key combination do nothing.
    if (oldKeycomb.equal(newKeycomb))
        return RC_OLD_EQUAL_NEW;

    VoidFunc voidfunc = getVoidFunc_(oldKeycomb);
    ArgFuncArg argfuncarg = getArgFuncArg_(oldKeycomb);

    // If the old key combination is not exists do nothing.
    if (voidfunc == nullptr && argfuncarg.first == nullptr)
        return RC_NOT_FIND;

    // If the new key combination already exists do nothing.
    if (getVoidFunc_(newKeycomb) || getArgFuncArg_(newKeycomb).first)
        return RC_ALREADY_EXISTED;

    Task tsk;
    tsk.opc = Task::REPLACE;
    tsk.replace.oldKeycomb = oldKeycomb;
    tsk.replace.newKeycomb = newKeycomb;

    setTask_(tsk);
    int rslt = waitTaskFinished_();

    if (rslt == RC_SUCCESS)
    {
        removeFunc_(oldKeycomb);

        if (voidfunc)
            addFunc_(newKeycomb, voidfunc);
        else
            addFunc_(newKeycomb, argfuncarg.first, argfuncarg.second);
    }

    return rslt;
}

void RegGlobalHotkey::work_()
{
    MSG msg;

    while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_HOTKEY)
        {
            int keyid = static_cast<int>(msg.wParam);
            if (keyIdKeycombs_.find(keyid) != keyIdKeycombs_.end())
            {
                VoidFunc voidfunc = getVoidFunc_(keyIdKeycombs_[keyid]);
                if (voidfunc)
                {
                    voidfunc();
                }
                else
                {
                    ArgFuncArg argfuncarg = getArgFuncArg_(keyIdKeycombs_[keyid]);
                    if (argfuncarg.first)
                        argfuncarg.first(argfuncarg.second);
                }
            }
        }

        ::TranslateMessage(&msg);
        ::DispatchMessageA(&msg);
    }
}

// @TODO How to fix the problem when UnregisterHotkey has error.
GBHK_NODISCARD int RegGlobalHotkey::end_()
{
    int rslt = RC_SUCCESS;

    // Unregister all hotkey.
    for (const auto& var : keyIdKeycombs_)
    {
        if (!::UnregisterHotKey(NULL, var.first))
            rslt = ::GetLastError();
    }

    // Reset the members.
    keyId_ = 0;
    keyIdKeycombs_.clear();

    return rslt;
}

GBHK_NODISCARD int RegGlobalHotkey::add_(const KeyCombination& keycomb)
{
    if (::RegisterHotKey(NULL, keyId_, keycomb.nativeModifiers(), keycomb.nativeKey()))
    {
        keyIdKeycombs_.insert({ keyId_, keycomb });
        keyId_++;
        return RC_SUCCESS;
    }
    return ::GetLastError();
}

GBHK_NODISCARD int RegGlobalHotkey::remove_(const KeyCombination& keycomb)
{
    for (const auto& var : keyIdKeycombs_)
    {
        if (var.second == keycomb)
        {
            if (::UnregisterHotKey(NULL, var.first))
            {
                keyIdKeycombs_.erase(var.first);
                return RC_SUCCESS;
            }
            return ::GetLastError();
        }
    }

    return RC_NOT_FIND;
}

GBHK_NODISCARD int RegGlobalHotkey::replace_(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb)
{
    for (const auto& var : keyIdKeycombs_)
    {
        if (var.second == oldKeycomb)
        {
            if (::UnregisterHotKey(NULL, var.first))
            {
                int keyid = var.first;
                keyIdKeycombs_.erase(keyid);

                if (::RegisterHotKey(NULL, keyid, newKeycomb.nativeModifiers(), newKeycomb.nativeKey()))
                {
                    keyIdKeycombs_.insert({ keyid, newKeycomb });
                    return RC_SUCCESS;
                }
                return ::GetLastError();
            }
            else
            {
                return ::GetLastError();
            }
        }
    }

    return RC_NOT_FIND;
}

void RegGlobalHotkey::setTask_(const Task& task)
{
    mtx_.lock();
    task_ = task;
    mtx_.unlock();

    hasTask_ = true;
    taskIsFinished_ = false;
}

GBHK_NODISCARD RegGlobalHotkey::Task RegGlobalHotkey::getTask_()
{
    Task tsk;

    if (hasTask_)
    {
        mtx_.lock();
        tsk = task_;
        mtx_.unlock();

        hasTask_ = false;
    }

    return tsk;
}

GBHK_NODISCARD int RegGlobalHotkey::waitTaskFinished_() const
{
    while (!taskIsFinished_)
        sleep(1);

    return taskResult_;
}

VoidFunc RegGlobalHotkey::getVoidFunc_(const KeyCombination& keycomb)
{
    VoidFunc rslt = nullptr;

    std::lock_guard<std::mutex> lock(mtx_);

    if (voidFuncs_.find(keycomb) != voidFuncs_.end())
        rslt = voidFuncs_[keycomb];

    return rslt;
}

ArgFuncArg RegGlobalHotkey::getArgFuncArg_(const KeyCombination& keycomb)
{
    ArgFuncArg rslt = { nullptr, nullptr };

    std::lock_guard<std::mutex> lock(mtx_);

    if (argFuncArgs_.find(keycomb) != argFuncArgs_.end())
        rslt = argFuncArgs_[keycomb];

    return rslt;
}

void RegGlobalHotkey::addFunc_(const KeyCombination& keycomb, VoidFunc callbackFunc)
{
    std::lock_guard<std::mutex> lock(mtx_);
    voidFuncs_.insert({ keycomb, callbackFunc });
}

void RegGlobalHotkey::addFunc_(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg)
{
    std::lock_guard<std::mutex> lock(mtx_);
    argFuncArgs_.insert({ keycomb, { callbackFunc, arg } });
}

void RegGlobalHotkey::removeFunc_(const KeyCombination& keycomb)
{
    std::lock_guard<std::mutex> lock(mtx_);

    voidFuncs_.erase(keycomb);
    argFuncArgs_.erase(keycomb);
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_NOREGISTER
