#ifndef GLOBAL_HOTKEY_TYPEALIAS_HPP
#define GLOBAL_HOTKEY_TYPEALIAS_HPP

#include <stddef.h> // size_t
#include <string>   // string
#include <utility>  // pair
#include <vector>   // vector

namespace gbhk
{

using Arg           = void*;
using VoidFunc      = void (*)();
using ArgFunc       = void (*)(Arg);
using ArgFuncArg    = std::pair<ArgFunc, Arg>;

using String        = std::string;
using Strings       = std::vector<String>;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_TYPEALIAS_HPP
