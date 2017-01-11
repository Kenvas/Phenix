#pragma once


#include <chrono>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#undef WIN32_LEAN_AND_MEAN

#include <gl/GL.h>

#ifdef __cplusplus
}
#endif


// third party begin
#include "termcolor/termcolor.hpp"

#include "fmt/format.h"
#include "fmt/time.h"
// third party end


//#include "kv/predef/IncludeAll.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"