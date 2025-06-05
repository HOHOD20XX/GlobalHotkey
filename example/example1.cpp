#include <atomic>       // atomic
#include <iostream>     // cout, endl
#include <stdexcept>    // runtime_error
#include <string>       // to_string()

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
#include <windows.h>    // Window event loop.
#endif // GLOBAL_HOTKEY_EXAMPLE_USE_HOOK

#include <global_hotkey/global_hotkey.hpp>

#define THROW_RT_ERR(errmsg, code) throw std::runtime_error(errmsg + std::to_string(code))

int main()
{
    gbhk::GlobalHotkeyBase* hotkeyManager = nullptr;

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    hotkeyManager = &gbhk::HookGlobalHotkey::getInstance();
#else
    hotkeyManager = &gbhk::RegGlobalHotkey::getInstance();
#endif // GLOBAL_HOTKEY_EXAMPLE_USE_HOOK

    gbhk::KeyCombination hotkey1(gbhk::CTRL, 'J', false);
    gbhk::KeyCombination hotkey2(gbhk::CTRL | gbhk::SHIFT, 'J', true);
    gbhk::KeyCombination hotkey3(gbhk::CTRL | gbhk::SHIFT, gbhk::KY_DELETE, false);

    std::cout << "The Hotkeys: " << "\n"
              << hotkey1.toString(true, true) << "\n"
              << hotkey2.toString(true, true) << "\n"
              << std::endl;
    std::cout << "Press " << hotkey3.toString(true) << " To Exit!" << std::endl;

    auto rtn = hotkeyManager->start();
    if (rtn != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to start the hotkey: ", rtn);
    }

    rtn = hotkeyManager->add(hotkey1, []()
    {
        std::cout << "hotkey1 triggered" << std::endl;
    });
    if (rtn != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to add the hotkey: ", rtn);
    }

    rtn = hotkeyManager->add(hotkey2, []()
    {
        std::cout << "hotkey2 triggered" << std::endl;
    });
    if (rtn != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to add the hotkey: ", rtn);
    }

    std::atomic<bool> shouldClose(false);

    rtn = hotkeyManager->add(hotkey3, [](void* shouldClose)
    {
        std::cout << "hotkey3 triggered" << std::endl;
        std::cout << "exiting..." << std::endl;

        *static_cast<std::atomic<bool>*>(shouldClose) = true;
    }, &shouldClose);
    if (rtn != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to add the hotkey: ", rtn);
    }

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    MSG msg = {};
    while (!shouldClose)
    {
        if (::PeekMessageA(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) != 0)
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }

        gbhk::sleep(10);
    }
#else
    while (!shouldClose)
    {
        // Pass
        continue;
    }
#endif // GLOBAL_HOTKEY_EXAMPLE_USE_HOOK

    rtn = hotkeyManager->end();
    if (rtn != gbhk::RC_SUCCESS)
    {
        THROW_RT_ERR("Failed to end the hotkey: ", rtn);
    }

    std::cout << "Success to exit." << std::endl;

    return 0;
}
