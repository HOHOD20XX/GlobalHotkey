#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_WIN_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_WIN_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <windows.h>

namespace gbhk
{

namespace kbhook
{

class _KBHMPrivateWin final : public _KBHMPrivate
{
public:
    _KBHMPrivateWin();
    ~_KBHMPrivateWin();

private:
    int specializedEnd() override;
    int doBeforeLoop() override;
    int doAfterLoop() override;

    void eachCycleDo() override;

    friend LRESULT LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    MSG msg = {0};
    HHOOK hhook = nullptr;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_WIN_HPP
