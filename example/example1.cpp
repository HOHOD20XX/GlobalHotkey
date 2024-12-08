#include <atomic>
#include <iostream>
#include <stdexcept>

#include <global_hotkey/global_hotkey.hpp>

int main()
{
    gbhk::BaseGlobalHotkey *hotkeyManager = nullptr;

    hotkeyManager = &gbhk::RegGlobalHotkey::getInstance();

    gbhk::KeyCombination hotkey1 = { gbhk::CTRL, 'S', false };
    gbhk::KeyCombination hotkey2(gbhk::CTRL | gbhk::SHIFT, 'S', true);
    gbhk::KeyCombination hotkey3(gbhk::CTRL | gbhk::SHIFT, gbhk::KY_DELETE, false);

    auto rtn = hotkeyManager->start();
    if (rtn != 0)
        throw std::runtime_error("Failed to start the hotkey.");

    rtn = hotkeyManager->add(hotkey1, [] () {
        std::cout << "hotkey1 triggered" << std::endl;
    });
    if (rtn != 0)
        throw std::runtime_error("Failed to add the hotkey.");

    rtn = hotkeyManager->add(hotkey2, [] () {
        std::cout << "hotkey2 triggered" << std::endl;
    });
    if (rtn != 0)
        throw std::runtime_error("Failed to add the hotkey.");

    std::atomic<bool> shouldClose(false);

    rtn = hotkeyManager->add(hotkey3, [] (void* shouldClose) {
        std::cout << "hotkey3 triggered" << std::endl;
        std::cout << "exiting..." << std::endl;

        *static_cast<std::atomic<bool>*>(shouldClose) = true;
    }, &shouldClose);
    if (rtn != 0)
        throw std::runtime_error("Failed to add the hotkey.");

    while (!shouldClose) {
        // Pass
        continue;
    }

    rtn = hotkeyManager->end();
    if (rtn != 0)
        throw std::runtime_error("Failed to end the hotkey.");

    std::cout << "Success to exit." << std::endl;

    return 0;
}
