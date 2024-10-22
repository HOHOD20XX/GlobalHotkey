#ifndef CORE_HPP
#define CORE_HPP

#include <stddef.h>     // size_t

#include "macros.hpp"
#include "typealias.hpp"

#ifdef _MSVC_LANG
#define GBHK_CPPVER     _MSVC_LANG
#else
#define GBHK_CPPVER     __cplusplus
#endif // _MSVC_LANG

#if GBHK_CPPVER >= 201703L
#define GBHK_CPP17
#endif // GBHK_CPPVER >= 201703L

#ifdef GBHK_CPP17
#define GBHK_NODISCARD  [[nodiscard]]
#else
#define GBHK_NODISCARD
#endif // GBHK_CPP17

#if defined(_WIN32)
#define GBHK_WIN
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define GBHK_MAC
#else
#error "Unsupported apple platform"
#endif // TARGET_OS_MAC
#elif __linux__
#define GBHK_LINUX
#else 
#error "Unsupported platform"
#endif // _WIN32

#endif // !CORE_HPP
