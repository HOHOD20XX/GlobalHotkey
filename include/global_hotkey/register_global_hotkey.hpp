#ifndef REGISTER_GLOBAL_HOTKEY_HPP
#define REGISTER_GLOBAL_HOTKEY_HPP

#include "global_hotkey_base.hpp"

#include <atomic>
#include <unordered_map>

namespace gbhk
{

class RegGlobalHotkey final : public GlobalHotkeyBase
{
public:
    static RegGlobalHotkey &getInstance();

    GBHK_NODISCARD uint start();
    GBHK_NODISCARD uint end();
    GBHK_NODISCARD uint add(const KeyCombination &keycomb, VoidFunc func);
    GBHK_NODISCARD uint add(const KeyCombination &keycomb, ArgFunc func, Arg arg);
    GBHK_NODISCARD uint remove(const KeyCombination &keycomb);
    GBHK_NODISCARD uint replace(const KeyCombination &oldKeycomb,
                                const KeyCombination &newKeycomb);

private:
    struct Task
    {
        enum OpCode : uchar
        {
            NONE,
            ADD,
            REMOVE,
            REPLACE,
            END
        };

        OpCode opc;
        union
        {
            struct
            {
                KeyCombination keycomb;
            } add;
            struct
            {
                KeyCombination keycomb;
            } remove;
            struct
            {
                KeyCombination oldKeycomb;
                KeyCombination newKeycomb;
            } replace;
        };

        Task() : opc(NONE), add() {}
    };

    RegGlobalHotkey();
    ~RegGlobalHotkey();
    RegGlobalHotkey(const RegGlobalHotkey &other) = delete;
    RegGlobalHotkey &operator=(const RegGlobalHotkey &other) = delete;

    void work_();
    GBHK_NODISCARD uint end_();
    GBHK_NODISCARD uint add_(const KeyCombination &keycomb);
    GBHK_NODISCARD uint remove_(const KeyCombination &keycomb);
    GBHK_NODISCARD uint replace_(const KeyCombination &oldKeycomb,
                                 const KeyCombination &newKeycomb);

    void setTask_(const Task &task);
    GBHK_NODISCARD Task getTask_();
    GBHK_NODISCARD uint waitTaskFinished_();

    VoidFunc getVoidFunc_(const KeyCombination &keycomb);
    ArgFuncArg getArgFuncArg_(const KeyCombination &keycomb);

    void addFunc_(const KeyCombination &keycomb, VoidFunc func);
    void addFunc_(const KeyCombination &keycomb, ArgFunc func, Arg arg);
    void removeFunc_(const KeyCombination &keycomb);

    Task task_;
    std::atomic<bool> hasTask_;
    std::atomic<bool> taskIsFinished_;
    std::atomic<uint> taskResult_;
    std::atomic<uint> keyId_;
    std::mutex mtx_;
    // Mapping of the key combination and func.
    std::unordered_map<KeyCombination, VoidFunc, KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, ArgFuncArg, KeyCombination::Hash> argFuncArgs_;
    std::unordered_map<uint, KeyCombination> keyIdKeycombs_;
};

}

#endif // !REGISTER_GLOBAL_HOTKEY_HPP
