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

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
