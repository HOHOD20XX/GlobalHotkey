#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP

#include "key_combination.hpp"

namespace gbhk
{

class _GlobalHotkeyManagerPrivate;

class GBHK_API GlobalHotkeyManager
{
public:
    GlobalHotkeyManager(const GlobalHotkeyManager&) = delete;
    GlobalHotkeyManager& operator=(const GlobalHotkeyManager&) = delete;

    /// @brief Start the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int start();
    /// @brief End the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    /// @note All hotkeys will be removed.
    int end();
    /// @brief Add a hotkey to the global hotkey manager.
    /// @param autoRepeat Whether the hotkey should be auto-repeated when held down.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat = false);
    /// @overload
    int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat = false);
    /// @brief Remove a hotkey from the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int remove(const KeyCombination& kc);
    /// @brief Remove all hotkeys in the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int removeAll();
    /// @brief Replace a hotkey with another one.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    /// @brief Set whether a hotkey should be auto-repeated when held down.
    /// @return A #ReturnCode or a platfrom-specific error code.
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    /// @return Always #RC_SUCCESS.
    int setCycleTime(size_t milliseconds);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;

protected:
    explicit GlobalHotkeyManager(_GlobalHotkeyManagerPrivate* p);
    virtual ~GlobalHotkeyManager();

    _GlobalHotkeyManagerPrivate* p_ = nullptr;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
