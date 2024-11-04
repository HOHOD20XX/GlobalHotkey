#ifndef GLOBAL_HOTKEY_CORE_HPP
#define GLOBAL_HOTKEY_CORE_HPP

#include <stddef.h>     // size_t

#include "constants.hpp"
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

namespace gbhk
{

#if defined(_GLOBAL_HOTKEY_WIN)
constexpr const char* _MODKEY_TEXT_META = _MODKEY_TEXT_WIN;
#elif defined(_GLOBAL_HOTKEY_MAC)
constexpr const char* _MODKEY_TEXT_META = _MODKEY_TEXT_CMD;
#elif defined(_GLOBAL_HOTKEY_LINUX)
constexpr const char* _MODKEY_TEXT_META = _MODKEY_TEXT_SUPER;
#else
constexpr const char* _MODKEY_TEXT_META = "";
#endif // _GLOBAL_HOTKEY_WIN

}

#endif // !GLOBAL_HOTKEY_CORE_HPP
