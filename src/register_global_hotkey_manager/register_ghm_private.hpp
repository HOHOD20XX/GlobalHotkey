#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "../global_hotkey_manager/ghm_private.hpp"

namespace gbhk
{

class _RegisterGHMPrivate : public _GHMPrivate
{
public:
    _RegisterGHMPrivate();
    ~_RegisterGHMPrivate();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
