#include <cstdio>               // printf
#include <atomic>               // atomic
#include <condition_variable>   // condition_variable
#include <mutex>                // mutex
#include <stdexcept>            // runtime_error

#include <global_hotkey/global_hotkey.hpp>

#define THROW_RT_ERR(errmsg, code) throw std::runtime_error(errmsg + gbhk::returnCodeMessage(code))

void hotkeyTriggered1()
{
    printf("Hotkey 1 be triggered\n");
}

void hotkeyTriggered2()
{
    printf("Hotkey 2 be triggered\n");
}

int main()
{
#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    gbhk::GlobalHotkeyManager& ghm = gbhk::HookGlobalHotkeyManager::getInstance();
#else
    gbhk::GlobalHotkeyManager& ghm = gbhk::RegisterGlobalHotkeyManager::getInstance();
#endif // GLOBAL_HOTKEY_EXAMPLE_USE_HOOK

    gbhk::KeyCombination hotkey1(gbhk::CTRL, 'J');
    gbhk::KeyCombination hotkey2(gbhk::CTRL | gbhk::SHIFT, 'J');
    gbhk::KeyCombination hotkey3(gbhk::CTRL | gbhk::SHIFT, gbhk::Key_Delete);

    printf("Hotkeys:\n1: %s\n2: %s\n3: %s\n",
           hotkey1.toString(true).c_str(),
           hotkey2.toString(true).c_str(),
           hotkey3.toString(true).c_str());
    printf("Press the hotkey 3 to exit.\n\n");

    int rc = ghm.start();
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to start the Global Hotkey Manager: ", rc);

    rc = ghm.add(hotkey1, &hotkeyTriggered1);
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to add the hotkey 1: ", rc);

    // The hotkey 2 is auto repeat.
    rc = ghm.add(hotkey2, &hotkeyTriggered2, true);
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to add the hotkey 2: ", rc);

    std::atomic<bool> shouldClose(false);
    std::condition_variable cv;
    rc = ghm.add(hotkey3, [&]()
    {
        printf("Hotkey 3 be triggered\n");
        shouldClose = true;
        cv.notify_all();
    });
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to add the hotkey: ", rc);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cv.wait(lock, [&]() { return shouldClose.load(); });
    lock.unlock();

    printf("Ending...\n");
    rc = ghm.end();
    // The follow code forever not throw exception actually.
    if (rc != gbhk::RC_SUCCESS)
        THROW_RT_ERR("Failed to end the Global Hotkey Manager: ", rc);
    printf("The Global Hotkey Manager is ended\n");

    return 0;
}
