#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <iostream>             // cout, endl
#include <mutex>                // mutex
#include <stdexcept>            // runtime_error

#include <global_hotkey/global_hotkey.hpp>

#define THROW_RT_ERR(errmsg, code) throw std::runtime_error(errmsg + gbhk::returnCodeMessage(code))

int main()
{
#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    gbhk::GlobalHotkeyManager& hotkeyManager = gbhk::HookGHM::getInstance();
#else
    gbhk::GlobalHotkeyManager& hotkeyManager = gbhk::RegisterGHM::getInstance();
#endif // GLOBAL_HOTKEY_EXAMPLE_USE_HOOK

    gbhk::KeyCombination hotkey1(gbhk::MODI_CTRL, 'J');
    gbhk::KeyCombination hotkey2(gbhk::MODI_CTRL | gbhk::MODI_SHIFT, 'J');
    gbhk::KeyCombination hotkey3(gbhk::MODI_CTRL | gbhk::MODI_SHIFT, gbhk::Key_Delete);

    std::cout << "The Hotkeys: " << "\n"
              << hotkey1.toString(true) << "\n"
              << hotkey2.toString(true) << "\n"
              << std::endl;
    std::cout << "Press " << hotkey3.toString(true) << " To Exit!" << std::endl;

    auto rtn = hotkeyManager.start();
    if (rtn != gbhk::RC_SUCCESS)    THROW_RT_ERR("Failed to start the hotkey: ", rtn);

    rtn = hotkeyManager.add(hotkey1, []()
    {
        std::cout << "hotkey1 triggered" << std::endl;
    });
    if (rtn != gbhk::RC_SUCCESS)    THROW_RT_ERR("Failed to add the hotkey: ", rtn);

    rtn = hotkeyManager.add(hotkey2, []()
    {
        std::cout << "hotkey2 triggered" << std::endl;
    }, true);
    if (rtn != gbhk::RC_SUCCESS)    THROW_RT_ERR("Failed to add the hotkey: ", rtn);

    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> shouldClose(false);
    rtn = hotkeyManager.add(hotkey3, [&]()
    {
        std::cout << "hotkey3 triggered" << std::endl;
        std::cout << "exiting..." << std::endl;
        shouldClose = true;
        cv.notify_all();
    });
    if (rtn != gbhk::RC_SUCCESS)    THROW_RT_ERR("Failed to add the hotkey: ", rtn);

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&shouldClose]() { return shouldClose.load(); });

    rtn = hotkeyManager.end();
    if (rtn != gbhk::RC_SUCCESS)    THROW_RT_ERR("Failed to end the hotkey: ", rtn);

    std::cout << "Success to exit." << std::endl;

    return 0;
}
