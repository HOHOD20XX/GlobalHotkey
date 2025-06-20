#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

_RegisterGHMPrivate::_RegisterGHMPrivate() : taskFinished(false), taskReturnCode(RC_SUCCESS) {}

_RegisterGHMPrivate::~_RegisterGHMPrivate() = default;

int _RegisterGHMPrivate::specializedStart()
{
    return RC_SUCCESS;
}

int _RegisterGHMPrivate::specializedEnd()
{
    Task task;
    task.op = Task::END;

    setTask(task);
    int rc = waitTaskFinished();

    task = Task();

    return rc;
}

int _RegisterGHMPrivate::specializedAdd(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{
    if (isInWorkThread())       return RC_CHANGE_GH_IN_WRONG_THREAD;

    Task task;
    task.op = Task::ADD;
    task.add.kc = kc;
    task.add.autoRepeat = autoRepeat;

    setTask(task);
    int rc = waitTaskFinished();
    return rc;
}

int _RegisterGHMPrivate::specializedRemove(const KeyCombination& kc)
{
    if (isInWorkThread())       return RC_CHANGE_GH_IN_WRONG_THREAD;

    Task task;
    task.op = Task::REMOVE;
    task.remove.kc = kc;

    setTask(task);
    int rc = waitTaskFinished();
    return RC_SUCCESS;
}

int _RegisterGHMPrivate::specializedRemoveAll()
{
    if (isInWorkThread())       return RC_CHANGE_GH_IN_WRONG_THREAD;

    Task task;
    task.op = Task::REMOVE_ALL;

    setTask(task);
    int rc = waitTaskFinished();
    return RC_SUCCESS;
}

int _RegisterGHMPrivate::specializedReplace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (isInWorkThread())       return RC_CHANGE_GH_IN_WRONG_THREAD;

    Task task;
    task.op = Task::REPLACE;
    task.replace.oldKc = oldKc;
    task.replace.newKc = newKc;

    setTask(task);
    int rc = waitTaskFinished();
    return rc;
}

int _RegisterGHMPrivate::specializedSetAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    if (isInWorkThread())       return RC_CHANGE_GH_IN_WRONG_THREAD;

    Task task;
    task.op = Task::SET_AUTO_REPEAT;
    task.setAutoRepeat.kc = kc;
    task.setAutoRepeat.autoRepeat = autoRepeat;

    setTask(task);
    int rc = waitTaskFinished();
    return rc;
}

void _RegisterGHMPrivate::eachCycleDo()
{
    Task task = getTask();
    if (task.op != Task::NONE)
    {
        switch (task.op)
        {
            case Task::END:
                taskReturnCode = workOfEnd();
                break;
            case Task::ADD:
                taskReturnCode = workOfAdd(task.add.kc, task.add.autoRepeat);
                break;
            case Task::REMOVE:
                taskReturnCode = workOfRemove(task.remove.kc);
                break;
            case Task::REPLACE:
                taskReturnCode = workOfReplace(task.replace.oldKc, task.replace.newKc);
                break;
            case Task::SET_AUTO_REPEAT:
                taskReturnCode = workOfSetAutoRepeat(task.setAutoRepeat.kc, task.setAutoRepeat.autoRepeat);
                break;
            default:
                taskReturnCode = RC_SUCCESS;
                break;
        }
        taskFinished = true;
        cvTaskFinished.notify_all();
    }

    if (task.op == Task::END)
        return;

    work();
}

void _RegisterGHMPrivate::setTask(const Task& task)
{
    taskFinished = false;
    taskReturnCode = RC_SUCCESS;
    std::lock_guard<std::mutex> lock(mtx);
    this->task = task;
}

_RegisterGHMPrivate::Task _RegisterGHMPrivate::getTask()
{
    std::lock_guard<std::mutex> lock(mtx);
    return task;
}

int _RegisterGHMPrivate::waitTaskFinished()
{
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvTaskFinished.wait(lock, [this]() { return taskFinished.load(); });
    lock.unlock();
    taskFinished = false;
    return taskReturnCode;
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
