#ifndef GLOBAL_HOTKEY_HOOK_GHM_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API HookGHM final : public GlobalHotkeyManager
{
public:
    static HookGHM& getInstance();

    int setDebouncedTime(size_t milliseconds);

private:
    HookGHM();
    ~HookGHM();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_HPP
