#include "PrecompiledHeader.hpp"
#include "kv/log/LogWrap.hpp"

#include "../_namespace/begin"
std::shared_ptr<spdlog::logger> console;// = spdlog::stdout_color_mt("console");
std::shared_ptr<spdlog::logger> simple;

static auto init = []()
{
    console = spdlog::stdout_color_mt("console");
    console->set_pattern("%T.%f %L [%t] %v");

    simple = spdlog::stdout_color_mt("simple");
    simple->set_pattern("%v");

    return true;
}();
#include "../_namespace/end"