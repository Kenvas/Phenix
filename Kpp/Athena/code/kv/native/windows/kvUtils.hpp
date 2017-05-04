#pragma once

#include "kv/predef/OS.hpp"

#if defined(KV_OS_WINDOWS)
#   include "kvUtilsWindows.hpp"
#else
#   error "utils functions not implemented."
#endif