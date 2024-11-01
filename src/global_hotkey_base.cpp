#include <global_hotkey/global_hotkey_base.hpp>

namespace gbhk
{

void sleep(ullong millisecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

GlobalHotkeyBase::GlobalHotkeyBase() :
    isRunning_(false), shouldClose_(false), intervalTime_(10),
    timePoint_(std::chrono::steady_clock::time_point())
{}

GlobalHotkeyBase::~GlobalHotkeyBase() {}


bool GlobalHotkeyBase::isRunning() const
{
    return isRunning_;
}

void GlobalHotkeyBase::setIntervalTime(ullong millisecond)
{
    intervalTime_ = millisecond;
}

void GlobalHotkeyBase::setTimePoint_()
{
    namespace chr = std::chrono;

    mtx_.lock();
    timePoint_ = chr::steady_clock::now() + chr::milliseconds(intervalTime_);
    mtx_.unlock();
}

void GlobalHotkeyBase::waitInterval_()
{
    mtx_.lock();
    std::this_thread::sleep_until(timePoint_);
    mtx_.unlock();
}

void GlobalHotkeyBase::setWorkThreadId_(const std::thread::id& id)
{
    mtx_.lock();
    workThreadId_ = id;
    mtx_.unlock();
}

std::thread::id GlobalHotkeyBase::getWorkThreadId_()
{
    mtx_.lock();
    std::thread::id id = workThreadId_;
    mtx_.unlock();

    return id;
}

}
