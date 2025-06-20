#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

_RegisterGHMPrivateMac::_RegisterGHMPrivateMac() = default;

_RegisterGHMPrivateMac::~_RegisterGHMPrivateMac() { end(); }

void _RegisterGHMPrivateMac::work()
{

}

int _RegisterGHMPrivateMac::workOfEnd()
{

}

int _RegisterGHMPrivateMac::workOfAdd(const KeyCombination& kc, bool autoRepeat)
{

}

int _RegisterGHMPrivateMac::workOfRemove(const KeyCombination& kc)
{

}

int _RegisterGHMPrivateMac::workOfRemoveAll()
{

}

int _RegisterGHMPrivateMac::workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc)
{

}

int _RegisterGHMPrivateMac::workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{

}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
