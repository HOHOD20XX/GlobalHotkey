#ifndef GLOBAL_HOTKEY_REGISTER_GHM_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API RegisterGHM final : public GlobalHotkeyManager
{
public:
    static RegisterGHM& getInstance();

private:
    RegisterGHM();
    ~RegisterGHM();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_HPP
