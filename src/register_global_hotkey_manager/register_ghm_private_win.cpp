#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

_RegisterGHMPrivateWin::_RegisterGHMPrivateWin() = default;

_RegisterGHMPrivateWin::~_RegisterGHMPrivateWin() { end(); }

void _RegisterGHMPrivateWin::work()
{
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            int hotkeyId = static_cast<int>(msg.wParam);
            if (hotkeyIdToKc.find(hotkeyId) != hotkeyIdToKc.end())
            {
                auto& kc = hotkeyIdToKc[hotkeyId];
                auto fn = getValue(kc).second;
                if (fn)
                    fn();
            }
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

int _RegisterGHMPrivateWin::workOfEnd()
{
    return workOfRemoveAll();
}

int _RegisterGHMPrivateWin::workOfAdd(const KeyCombination &kc, bool autoRepeat)
{
    int mod = nativeModifiers(kc.modifiers());
    mod |= (!autoRepeat ? MOD_NOREPEAT : 0);
    if (RegisterHotKey(NULL, hotkeyIndex, mod, nativeKey(kc.key())) != 0)
    {
        hotkeyIdToKc[hotkeyIndex] = kc;
        kcToHotkeyId[kc] = hotkeyIndex;
        hotkeyIndex++;
        return RC_SUCCESS;
    }
    return GetLastError();
}

int _RegisterGHMPrivateWin::workOfRemove(const KeyCombination &kc)
{
    int hotkeyId = kcToHotkeyId[kc];
    UnregisterHotKey(NULL, hotkeyId);
    hotkeyIdToKc.erase(hotkeyId);
    kcToHotkeyId.erase(kc);
    return RC_SUCCESS;
}

int _RegisterGHMPrivateWin::workOfRemoveAll()
{
    for (const auto& var : hotkeyIdToKc)
    {
        int hotkeyId = var.first;
        UnregisterHotKey(NULL, hotkeyId);
    }

    hotkeyIndex = 0;
    hotkeyIdToKc.clear();
    kcToHotkeyId.clear();

    return RC_SUCCESS;
}

int _RegisterGHMPrivateWin::workOfReplace(const KeyCombination &oldKc, const KeyCombination &newKc)
{
    bool autoRepeat = isAutoRepeat(oldKc);
    int rc = workOfRemove(oldKc);
    if (rc != RC_SUCCESS)
        return rc;
    rc = workOfAdd(newKc, autoRepeat);
    return rc;
}

int _RegisterGHMPrivateWin::workOfSetAutoRepeat(const KeyCombination &kc, bool autoRepeat)
{
    int rc = workOfRemove(kc);
    if (rc != RC_SUCCESS)
        return rc;
    rc = workOfAdd(kc, autoRepeat);
    return rc;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
