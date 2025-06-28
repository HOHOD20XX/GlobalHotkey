#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <windows.h>

namespace gbhk
{

class _RegisterGHMPrivateWin final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateWin();
    ~_RegisterGHMPrivateWin();

protected:
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    // The follow functions only be called in worker thread.
    void invoke(WPARAM wParam, LPARAM lParam);
    int nativeRegisterHotkey(WPARAM wParam, LPARAM lParam);
    int nativeUnregisterHotkey(WPARAM wParam, LPARAM lParam);

    std::atomic<DWORD> workerThreadId;
    std::condition_variable cvRegUnregRc;
    std::atomic<int> regUnregRc;
    std::atomic<int> hotkeyIndex;
    std::unordered_map<int, KeyCombination> hotkeyIdToKc;
    std::unordered_map<KeyCombination, int> kcToHotkeyId;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
