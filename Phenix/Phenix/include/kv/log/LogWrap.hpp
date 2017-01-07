#pragma once

#include "spdlog/spdlog.h"

namespace kv
{
    namespace log
    {
        namespace color = spdlog::color;

        extern std::shared_ptr<spdlog::logger> console;
        extern std::shared_ptr<spdlog::logger> simple;
    }
}