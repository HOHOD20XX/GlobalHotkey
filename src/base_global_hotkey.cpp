#include <global_hotkey/base_global_hotkey.hpp>

namespace gbhk
{

void sleep(ullong millisecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

BaseGlobalHotkey::BaseGlobalHotkey() :
    isRunning_(false), shouldClose_(false), intervalTime_(10), timePoint_(std::chrono::steady_clock::time_point())
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

    std::lock_guard<std::mutex> lock(mtx_);
    timePoint_ = chr::steady_clock::now() + chr::milliseconds(intervalTime_);
}

void BaseGlobalHotkey::waitInterval_()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::this_thread::sleep_until(timePoint_);
}

void BaseGlobalHotkey::setWorkThreadId_(const std::thread::id& id)
{
    std::lock_guard<std::mutex> lock(mtx_);
    workThreadId_ = id;
}

std::thread::id BaseGlobalHotkey::getWorkThreadId_()
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::thread::id id = workThreadId_;

    return id;
}

} // namespace gbhk
