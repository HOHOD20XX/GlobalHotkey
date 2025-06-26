#include <global_hotkey/global_hotkey_manager.hpp>

#include "ghm_private.hpp"

namespace gbhk
{

GlobalHotkeyManager::GlobalHotkeyManager(std::unique_ptr<_GHMPrivate> ptr) : ptr(std::move(ptr)) {}

GlobalHotkeyManager::~GlobalHotkeyManager() = default;

int GlobalHotkeyManager::start()
{ return ptr->start(); }

int GlobalHotkeyManager::end()
{ return ptr->end(); }

int GlobalHotkeyManager::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{ return ptr->add(kc, fn, autoRepeat); }

int GlobalHotkeyManager::remove(const KeyCombination& kc)
{ return ptr->remove(kc); }

int GlobalHotkeyManager::removeAll()
{ return ptr->removeAll(); }

int GlobalHotkeyManager::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{ return ptr->replace(oldKc, newKc); }

int GlobalHotkeyManager::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{ return ptr->setAutoRepeat(kc, autoRepeat); }

bool GlobalHotkeyManager::has(const KeyCombination& kc) const
{ return ptr->has(kc); }

bool GlobalHotkeyManager::isAutoRepeat(const KeyCombination& kc) const
{ return ptr->isAutoRepeat(kc); }

bool GlobalHotkeyManager::isRunning() const
{ return ptr->isRunning(); }

std::vector<KeyCombination> GlobalHotkeyManager::getAllKeyCombination() const
{ return ptr->getAllKeyCombination(); }

} // namespace gbhk
