#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <global_hotkey/hook_ghm.hpp>

#include "hook_ghm_private.hpp"

namespace gbhk
{

HookGHM& HookGHM::getInstance()
{
    static HookGHM instance;
    return instance;
}

HookGHM::HookGHM() :
    GlobalHotkeyManager(new _HookGHMPrivate())
{}

HookGHM::~HookGHM() = default;

int HookGHM::start()
{ return p_->start(); }

int HookGHM::end()
{ return p_->end(); }

int HookGHM::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{ return p_->add(kc, func, autoRepeat); }

int HookGHM::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{ return p_->add(kc, func, arg, autoRepeat); }

int HookGHM::remove(const KeyCombination& kc)
{ return p_->remove(kc); }

int HookGHM::removeAll()
{ return p_->removeAll(); }

int HookGHM::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return p_->replace(oldKc, newKc); }

int HookGHM::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return p_->setAutoRepeat(kc, autoRepeat); }

int HookGHM::setCycleTime(size_t milliseconds)
{ return p_->setCycleTime(milliseconds); }

int HookGHM::setDebouncedTime(size_t milliseconds)
{ return dynamic_cast<_HookGHMPrivate*>(p_)->setDebouncedTime(milliseconds); }

bool HookGHM::has(const KeyCombination& kc) const
{ return p_->has(kc); }

bool HookGHM::isAutoRepeat(const KeyCombination& kc) const
{ return p_->isAutoRepeat(kc); }

bool HookGHM::isRunning() const
{ return p_->isRunning(); }

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
