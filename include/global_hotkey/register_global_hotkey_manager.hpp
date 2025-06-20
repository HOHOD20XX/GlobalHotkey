#ifndef GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API RegisterGlobalHotkeyManager final : public GlobalHotkeyManager
{
public:
    static RegisterGlobalHotkeyManager& getInstance();

private:
    RegisterGlobalHotkeyManager();
    ~RegisterGlobalHotkeyManager();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP
