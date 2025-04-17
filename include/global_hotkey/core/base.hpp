#ifndef GLOBAL_HOTKEY_BASE_HPP
#define GLOBAL_HOTKEY_BASE_HPP

#include "typealias.hpp"

#ifdef _MSVC_LANG
    #define _GLOBAL_HOTKEY_CPPVERS     _MSVC_LANG
#else
    #define _GLOBAL_HOTKEY_CPPVERS     __cplusplus
#endif // _MSVC_LANG

#if _GLOBAL_HOTKEY_CPPVERS >= 201703L
    #define _GLOBAL_HOTKEY_CPP17
#endif // _GLOBAL_HOTKEY_CPPVERS >= 201703L

#ifdef _GLOBAL_HOTKEY_CPP17
    #define GBHK_NODISCARD  [[nodiscard]]
#else
    #define GBHK_NODISCARD
#endif // _GLOBAL_HOTKEY_CPP17

#if defined(_WIN32)
    #define GLOBAL_HOTKEY_WIN
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define GLOBAL_HOTKEY_MAC
    #else
        #error "Unsupported apple platform"
    #endif // TARGET_OS_MAC
#elif defined(__linux__)
    #define GLOBAL_HOTKEY_LINUX
#else 
    #error "Unsupported platform"
#endif // _WIN32

#ifdef GLOBAL_HOTKEY_WIN
    #ifdef GLOBAL_HOTKEY_BUILD_SHARED
        #define GBHK_API __declspec(dllexport)
    #elif defined(GLOBAL_HOTKEY_SHARED)
        #define GBHK_API __declspec(dllimport)
    #else
        #define GBHK_API
    #endif // GLOBAL_HOTKEY_BUILD_SHARED
#else
    #define GBHK_API __attribute__((visibility("default")))
#endif // GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_BASE_HPP
