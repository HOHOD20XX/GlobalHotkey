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

    int start() override;
    int end() override;
    int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat) override;
    int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat) override;
    int remove(const KeyCombination& kc) override;
    int removeAll() override;
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;
private:
    struct Task
    {
        enum Operator
        {
            NONE,
            ADD,
            REMOVE,
            REMOVE_ALL,
            REPLACE,
            SET_AUTO_REPEAT,
            END
        };

        Operator op;
        union
        {
            struct
            {
                KeyCombination kc;
                bool autoRepeat;
            } add;
            struct
            {
                KeyCombination kc;
            } remove;
            struct
            {
                KeyCombination oldKc;
                KeyCombination newKc;
            } replace;
            struct
            {
                KeyCombination kc;
                bool autoRepeat;
            } setAutoRepeat;
        };

        Task() : op(NONE), replace() {}
    };

    void work_();
    void handleEvent_();

    int end_();
    int add_(const KeyCombination& kc, bool autoRepeat);
    int remove_(const KeyCombination& kc);
    int removeAll_();
    int replace_(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setAutoRepeat_(const KeyCombination& kc, bool autoRepeat);

    void setTask_(const Task& task);
    Task getTask_();
    int waitTask_() const;

    Task task_;
    std::atomic<bool> hasTask_;
    std::atomic<bool> taskFinished_;
    std::atomic<int> taskResult_;
    std::atomic<int> hotkeyIndex_;

    Map<int, KeyCombination> hotkeyIdToKc_;
    Map<KeyCombination, int, KeyCombination::Hash> kcToHotkeyId_;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
