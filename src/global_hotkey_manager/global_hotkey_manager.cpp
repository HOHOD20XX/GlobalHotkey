#include <global_hotkey/global_hotkey_manager.hpp>

#include "global_hotkey_manager_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(_GlobalHotkeyManagerPrivate* p)
    : p_(p)
{}

GlobalHotkeyManager::~GlobalHotkeyManager()
{
    delete p_;
    p_ = nullptr;
}

int GlobalHotkeyManager::start()
{ return p_->start(); }

int GlobalHotkeyManager::end()
{ return p_->end(); }

int GlobalHotkeyManager::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{ return p_->add(kc, func, autoRepeat); }

int GlobalHotkeyManager::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{ return p_->add(kc, func, arg, autoRepeat); }

int GlobalHotkeyManager::remove(const KeyCombination& kc)
{ return p_->remove(kc); }

int GlobalHotkeyManager::removeAll()
{ return p_->removeAll(); }

int GlobalHotkeyManager::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return p_->replace(oldKc, newKc); }

int GlobalHotkeyManager::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return p_->setAutoRepeat(kc, autoRepeat); }

int GlobalHotkeyManager::setCycleTime(size_t milliseconds)
{ return p_->setCycleTime(milliseconds); }

bool GlobalHotkeyManager::has(const KeyCombination& kc) const
{ return p_->has(kc); }

bool GlobalHotkeyManager::isAutoRepeat(const KeyCombination& kc) const
{ return p_->isAutoRepeat(kc); }

bool GlobalHotkeyManager::isRunning() const
{ return p_->isRunning(); }

} // namespace gbhk
