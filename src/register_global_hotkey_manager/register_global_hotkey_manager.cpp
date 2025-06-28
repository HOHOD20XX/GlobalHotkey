#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include <global_hotkey/register_global_hotkey_manager.hpp>

#ifdef _GLOBAL_HOTKEY_WIN
    #include "register_ghm_private_win.hpp"
#elif defined(_GLOBAL_HOTKEY_MAC)
    #include "register_ghm_private_mac.hpp"
#elif defined(_GLOBAL_HOTKEY_LINUX)
    #include "register_ghm_private_x11.hpp"
#endif // _GLOBAL_HOTKEY_WIN

namespace gbhk
{

RegisterGlobalHotkeyManager& RegisterGlobalHotkeyManager::getInstance()
{
    static RegisterGlobalHotkeyManager instance;
    return instance;
}

RegisterGlobalHotkeyManager::RegisterGlobalHotkeyManager() :
#ifdef _GLOBAL_HOTKEY_WIN
    GlobalHotkeyManager(std::unique_ptr<_GHMPrivate>(new _RegisterGHMPrivateWin()))
#elif defined(_GLOBAL_HOTKEY_MAC)
    GlobalHotkeyManager(std::unique_ptr<_GHMPrivate>(new _RegisterGHMPrivateMac()))
#elif defined(_GLOBAL_HOTKEY_LINUX)
    GlobalHotkeyManager(std::unique_ptr<_GHMPrivate>(new _RegisterGHMPrivateX11()))
#endif // _GLOBAL_HOTKEY_WIN
{}

RegisterGlobalHotkeyManager::~RegisterGlobalHotkeyManager() = default;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
