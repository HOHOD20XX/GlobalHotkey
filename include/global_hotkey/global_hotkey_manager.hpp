#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP

#include <functional>   // function
#include <memory>       // unique_ptr
#include <vector>       // vector

#include "key_combination.hpp"

namespace gbhk
{

class _GHMPrivate;

class GBHK_API GlobalHotkeyManager
{
public:
    GlobalHotkeyManager(const GlobalHotkeyManager&) = delete;
    GlobalHotkeyManager& operator=(const GlobalHotkeyManager&) = delete;

    int start();
    int end();
    int add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat = false);
    int remove(const KeyCombination& kc);
    int removeAll();
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;
    std::vector<KeyCombination> getAllKeyCombination() const;

protected:
    explicit GlobalHotkeyManager(std::unique_ptr<_GHMPrivate> ptr);
    virtual ~GlobalHotkeyManager();

    std::unique_ptr<_GHMPrivate> ptr;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
