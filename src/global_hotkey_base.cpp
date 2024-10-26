#include <global_hotkey/global_hotkey_base.hpp>

#include <chrono>

namespace gbhk
{

GlobalHotkeyBase::GlobalHotkeyBase() :
    isRunning_(false), shouldClose_(false), delay_(10)
{}

GlobalHotkeyBase::~GlobalHotkeyBase() {}

bool GlobalHotkeyBase::isRunning() const
{
    return isRunning_;
}

void GlobalHotkeyBase::setDelay(uint ms)
{
    delay_ = ms;
}

void GlobalHotkeyBase::sleep_(uint ms) const
{
    std::this_thread::sleep_for(std::chrono::microseconds(ms));
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
