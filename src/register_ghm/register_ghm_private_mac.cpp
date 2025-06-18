#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

namespace gbhk
{

_RegisterGHMPrivateMac::_RegisterGHMPrivateMac() = default;

_RegisterGHMPrivateMac::~_RegisterGHMPrivateMac()
{
    end();
}

int _RegisterGHMPrivateMac::start()
{

}

int _RegisterGHMPrivateMac::end()
{

}

int _RegisterGHMPrivateMac::add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat)
{

}

int _RegisterGHMPrivateMac::add(const KeyCombination& kc, std::function<void()>&& fn, bool autoRepeat)
{

}

int _RegisterGHMPrivateMac::remove(const KeyCombination& kc)
{

}

int _RegisterGHMPrivateMac::removeAll()
{

}

int _RegisterGHMPrivateMac::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{

}

int _RegisterGHMPrivateMac::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{

}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
