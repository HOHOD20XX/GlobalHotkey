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

} // namespace gbhk
