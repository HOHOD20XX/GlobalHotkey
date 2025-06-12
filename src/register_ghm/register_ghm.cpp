#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include <global_hotkey/register_ghm.hpp>

#ifdef _GLOBAL_HOTKEY_WIN
    #include "register_ghm_private_win.hpp"
#elif defined(_GLOBAL_HOTKEY_MAC)
    #include "register_ghm_private_mac.hpp"
#elif defined(_GLOBAL_HOTKEY_LINUX)
    #include "register_ghm_private_linux.hpp"
#endif // _GLOBAL_HOTKEY_WIN

namespace gbhk
{

RegisterGHM& RegisterGHM::getInstance()
{
    static RegisterGHM instance;
    return instance;
}

RegisterGHM::RegisterGHM() :
#ifdef _GLOBAL_HOTKEY_WIN
    GlobalHotkeyManager(new _RegisterGHMPrivateWin())
#elif defined(_GLOBAL_HOTKEY_MAC)
    GlobalHotkeyManager(new _RegisterGHMPrivateMac())
#elif defined(_GLOBAL_HOTKEY_LINUX)
    GlobalHotkeyManager(new _RegisterGHMPrivateLinux())
#endif // _GLOBAL_HOTKEY_WIN
{}

RegisterGHM::~RegisterGHM() = default;

int RegisterGHM::start()
{ return p_->start(); }

int RegisterGHM::end()
{ return p_->end(); }

int RegisterGHM::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{ return p_->add(kc, func, autoRepeat); }

int RegisterGHM::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{ return p_->add(kc, func, arg, autoRepeat); }

int RegisterGHM::remove(const KeyCombination& kc)
{ return p_->remove(kc); }

int RegisterGHM::removeAll()
{ return p_->removeAll(); }

int RegisterGHM::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return p_->replace(oldKc, newKc); }

int RegisterGHM::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return p_->setAutoRepeat(kc, autoRepeat); }

int RegisterGHM::setCycleTime(size_t milliseconds)
{ return p_->setCycleTime(milliseconds); }

bool RegisterGHM::has(const KeyCombination& kc) const
{ return p_->has(kc); }

bool RegisterGHM::isAutoRepeat(const KeyCombination& kc) const
{ return p_->isAutoRepeat(kc); }

bool RegisterGHM::isRunning() const
{ return p_->isRunning(); }

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
