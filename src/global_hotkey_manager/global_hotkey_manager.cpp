#include <global_hotkey/global_hotkey_manager.hpp>

#include "global_hotkey_manager_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(std::unique_ptr<_GlobalHotkeyManagerPrivate> p) :
    p_(std::move(p))
{}

GlobalHotkeyManager::~GlobalHotkeyManager() = default;

int GlobalHotkeyManager::start()
{ return p_->start(); }

int GlobalHotkeyManager::end()
{ return p_->end(); }

int GlobalHotkeyManager::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{ return p_->add(kc, fn, autoRepeat); }

int GlobalHotkeyManager::add(const KeyCombination& kc, std::function<void()>&& fn, bool autoRepeat)
{ return p_->add(kc, std::move(fn), autoRepeat); }

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
