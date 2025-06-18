#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <global_hotkey/utility.hpp>

#include "../key/key_private_x11.hpp"

namespace gbhk
{

_RegisterGHMPrivateLinux::_RegisterGHMPrivateLinux() = default;

_RegisterGHMPrivateLinux::~_RegisterGHMPrivateLinux()
{
    end();
}

int _RegisterGHMPrivateLinux::start()
{

}

int _RegisterGHMPrivateLinux::end()
{

}

int _RegisterGHMPrivateLinux::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{

}

int _RegisterGHMPrivateLinux::add(const KeyCombination& kc, std::function<void()>&& fn, bool autoRepeat)
{

}

int _RegisterGHMPrivateLinux::remove(const KeyCombination& kc)
{

}

int _RegisterGHMPrivateLinux::removeAll()
{

}

int _RegisterGHMPrivateLinux::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{

}

int _RegisterGHMPrivateLinux::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{

}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
