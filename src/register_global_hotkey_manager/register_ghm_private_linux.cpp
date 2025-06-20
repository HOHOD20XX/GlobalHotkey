#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

#include "../key/key_private_x11.hpp"

namespace gbhk
{

_RegisterGHMPrivateLinux::_RegisterGHMPrivateLinux() = default;

_RegisterGHMPrivateLinux::~_RegisterGHMPrivateLinux() { end(); }

void _RegisterGHMPrivateLinux::work()
{

}

int _RegisterGHMPrivateLinux::workOfEnd()
{

}

int _RegisterGHMPrivateLinux::workOfAdd(const KeyCombination& kc, bool autoRepeat)
{

}

int _RegisterGHMPrivateLinux::workOfRemove(const KeyCombination& kc)
{

}

int _RegisterGHMPrivateLinux::workOfRemoveAll()
{

}

int _RegisterGHMPrivateLinux::workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc)
{

}

int _RegisterGHMPrivateLinux::workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{

}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
