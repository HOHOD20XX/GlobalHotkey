#include <global_hotkey/base_global_hotkey.hpp>

namespace gbhk
{

void sleep(ullong millisecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

BaseGlobalHotkey::BaseGlobalHotkey() :
    isRunning_(false), shouldClose_(false), intervalTime_(50), timePoint_(std::chrono::steady_clock::time_point())
{}

BaseGlobalHotkey::~BaseGlobalHotkey() {}

bool BaseGlobalHotkey::isRunning() const
{
    return isRunning_;
}

void BaseGlobalHotkey::setIntervalTime(ullong millisecond)
{
    intervalTime_ = millisecond;
}

void BaseGlobalHotkey::setTimePoint_()
{
    namespace chr = std::chrono;

    mtx_.lock();
    timePoint_ = chr::steady_clock::now() + chr::milliseconds(intervalTime_);
    mtx_.unlock();
}

void BaseGlobalHotkey::waitInterval_()
{
    mtx_.lock();
    std::this_thread::sleep_until(timePoint_);
    mtx_.unlock();
}

void BaseGlobalHotkey::setWorkThreadId_(const std::thread::id& id)
{
    mtx_.lock();
    workThreadId_ = id;
    mtx_.unlock();
}

std::thread::id BaseGlobalHotkey::getWorkThreadId_()
{
    mtx_.lock();
    std::thread::id id = workThreadId_;
    mtx_.unlock();

    return id;
}

} // namespace gbhk
