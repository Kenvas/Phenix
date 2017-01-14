#pragma once

#include "spdlog/spdlog.h"

#include "_namespace/begin"
namespace color = spdlog::color;

extern std::shared_ptr<spdlog::logger> console;
extern std::shared_ptr<spdlog::logger> simple;
#include "_namespace/end"