#ifndef GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API HookGlobalHotkeyManager final : public GlobalHotkeyManager
{
public:
    static HookGlobalHotkeyManager& getInstance();

private:
    HookGlobalHotkeyManager();
    ~HookGlobalHotkeyManager();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP
