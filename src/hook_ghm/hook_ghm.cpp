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
    GlobalHotkeyManager(std::unique_ptr<_GlobalHotkeyManagerPrivate>(new _HookGHMPrivate()))
{}

HookGHM::~HookGHM() = default;

int HookGHM::setDebouncedTime(size_t milliseconds)
{
    auto p = p_.get();
    return dynamic_cast<_HookGHMPrivate*>(p)->setDebouncedTime(milliseconds);
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
