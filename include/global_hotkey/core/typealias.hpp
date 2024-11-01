#ifndef TYPEALIAS_HPP
#define TYPEALIAS_HPP

#include <string>
#include <utility>  // std::pair
#include <vector>

namespace gbhk
{

using uchar = unsigned char;
using uint = unsigned int;
using llong = long long;
using ullong = unsigned long long;

using Arg = void*;
using VoidFunc = void (*)();
using ArgFunc = void (*)(Arg);
using ArgFuncArg = std::pair<ArgFunc, Arg>;

using String = std::string;
using Strings = std::vector<std::string>;

}

#endif // !TYPEALIAS_HPP
