#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

#include "fmt/format.h"

KV_QuickAddEntry
{
    fmt::print("Hello, {}!\n", "world");  // uses Python-like format string syntax
    fmt::print_colored(fmt::Color::CYAN , "Hello, {}!\n", "world"); // uses printf format string syntax
    fmt::print(stdout, "Hello, {}!\n", "world"); // uses printf format string syntax
    std::string s = fmt::format(fmt::Color::YELLOW, "{0}{1}{0}", "arbra", "cad");
    cout << s << endl;
};
