#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

namespace gbhk
{

namespace kbhook
{

_KBHMPrivateMac::_KBHMPrivateMac() = default;

_KBHMPrivateMac::~_KBHMPrivateMac() { end(); }

void _KBHMPrivateMac::eachCycleDo()
{

}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
