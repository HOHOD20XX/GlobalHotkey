#ifndef GLOBAL_HOTKEY_BASE_HPP
#define GLOBAL_HOTKEY_BASE_HPP

#include <cstddef>          // size_t

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

constexpr size_t _DEFAULT_CYCLE_TIME = (1000 / 30);
constexpr size_t _DEFAULT_KEYBOARD_HOOK_CYCLE_TIME = 10;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_BASE_HPP
