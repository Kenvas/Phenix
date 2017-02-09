#include "PrecompiledHeader.hpp"

#include "kv/log/IncludeAll.hpp"
#include <windows.h>

using namespace kv;

KV_QuickAddEntry
{
    log::debug("mouse enable: {0}", (bool)GetSystemMetrics(SM_MOUSEPRESENT))();
    log::debug("mouse wheel enable: {0}", (bool)GetSystemMetrics(SM_MOUSEWHEELPRESENT))();
    log::debug("mouse button count: {0}", GetSystemMetrics(SM_CMOUSEBUTTONS))();
    log::debug("mouse button swapped: {0}", (bool)GetSystemMetrics(SM_SWAPBUTTON))();
    return 0;
};