#ifndef GLOBAL_HOTKEY_KEY_PRIVATE_HPP
#define GLOBAL_HOTKEY_KEY_PRIVATE_HPP

#include <global_hotkey/key.hpp>

namespace gbhk
{

int nativeModifiers(const Modifiers& modifiers) noexcept;

int nativeKey(const Key& key) noexcept;

Modifiers getModifiersFromNativeModifiers(int nativeModifiers) noexcept;

Key getKeyFromNativeKey(int nativeKey) noexcept;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEY_PRIVATE_WIN_HPP
