#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include <global_hotkey/key.hpp>

#ifdef _GLOBAL_HOTKEY_LINUX

namespace gbhk
{

int x11Modifiers(const Modifiers& modifiers) noexcept;

int x11Keysym(const Key& key) noexcept;

Modifiers getModifiersFromX11Modifiers(int x11Modifiers) noexcept;

Key getKeyFromX11Keysym(int x11Keysym) noexcept;

}

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_X11_HPP
