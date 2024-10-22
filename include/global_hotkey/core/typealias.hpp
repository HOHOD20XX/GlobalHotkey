#ifndef TYPEALIAS_HPP
#define TYPEALIAS_HPP

#include <utility>  // std::pair
#include <vector>
#include <string>

namespace gbhk
{

typedef unsigned char uchar;
typedef unsigned int uint;

using Arg = void *;
using VoidFunc = void (*)();
using ArgFunc = void (*)(Arg);
using ArgFuncArg = std::pair<ArgFunc, Arg>;

using String = std::string;
using Strings = std::vector<std::string>;

}

#endif // !TYPEALIAS_HPP
