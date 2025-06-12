#ifndef GLOBAL_HOTKEY_BASE_HPP
#define GLOBAL_HOTKEY_BASE_HPP

#include <functional>       // hash
#include <cstddef>          // size_t
#include <string>           // string
#include <unordered_map>    // unordered_map
#include <utility>          // pair

#ifdef _MSVC_LANG
    #define _GLOBAL_HOTKEY_CPPVERS     _MSVC_LANG
#else
    #define _GLOBAL_HOTKEY_CPPVERS     __cplusplus
#endif // _MSVC_LANG

#if _GLOBAL_HOTKEY_CPPVERS < 201103L
    #error "Only support C++11 and above"
#endif // _GLOBAL_HOTKEY_CPPVERS < 201103L

#ifdef _WIN32
    #define _GLOBAL_HOTKEY_WIN
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define _GLOBAL_HOTKEY_MAC
    #else
        #error "Unsupported apple platform"
    #endif // TARGET_OS_MAC
#elif defined(__linux__)
    #define _GLOBAL_HOTKEY_LINUX
#else
    #error "Unsupported platform"
#endif // _WIN32

#ifdef _GLOBAL_HOTKEY_WIN
    #ifdef GLOBAL_HOTKEY_BUILD_SHARED
        #define GBHK_API __declspec(dllexport)
    #elif defined(GLOBAL_HOTKEY_SHARED)
        #define GBHK_API __declspec(dllimport)
    #else
        #define GBHK_API
    #endif // GLOBAL_HOTKEY_BUILD_SHARED
#else
    #define GBHK_API __attribute__((visibility("default")))
#endif // _GLOBAL_HOTKEY_WIN

namespace gbhk
{

using Arg           = void*;
using VoidFunc      = void (*)();
using ArgFunc       = void (*)(Arg);
using ArgFuncArg    = std::pair<ArgFunc, Arg>;

using String        = std::string;
template<typename T1, typename T2>
using Pair          = std::pair<T1, T2>;
template<typename Key, typename Value, typename Hash = std::hash<Key>>
using Map           = std::unordered_map<Key, Value, Hash>;

constexpr size_t _DEFAULT_CYCLE_TIME        = 10;
constexpr size_t _DEFAULT_DEBOUNCED_TIME    = 0;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_BASE_HPP
