#include "global_hotkey/global_hotkey_base.hpp"

namespace gbhk
{

void sleep(size_t millisecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

GlobalHotkeyBase::GlobalHotkeyBase() :
    isRunning_(false), shouldClose_(false), intervalTime_(10), timePoint_(std::chrono::steady_clock::time_point())
{}

GlobalHotkeyBase::~GlobalHotkeyBase() {}

bool GlobalHotkeyBase::isRunning() const
{
    return isRunning_;
}

void GlobalHotkeyBase::setIntervalTime(size_t millisecond)
{
    intervalTime_ = millisecond;
}

void GlobalHotkeyBase::setTimePoint_()
{
    namespace chr = std::chrono;

    std::lock_guard<std::mutex> lock(mtx_);
    timePoint_ = chr::steady_clock::now() + chr::milliseconds(intervalTime_);
}

void GlobalHotkeyBase::waitInterval_()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::this_thread::sleep_until(timePoint_);
}

void GlobalHotkeyBase::setWorkThreadId_(const std::thread::id& id)
{
    std::lock_guard<std::mutex> lock(mtx_);
    workThreadId_ = id;
}

std::thread::id GlobalHotkeyBase::getWorkThreadId_()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::thread::id id = workThreadId_;

    return id;
}

} // namespace gbhk
