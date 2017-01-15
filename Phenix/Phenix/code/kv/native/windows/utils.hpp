#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include "_namespace/begin"
namespace utils
{
void PrintErrorInfo() noexcept;
bool RegisterWindowClass(PCTSTR const class_name, WNDPROC const window_procdure) noexcept;
void UnregisterWindowClass(PCTSTR const class_name) noexcept;
char const * const GetWindowMessageName(size_t const message) noexcept;
}
#include "_namespace/end"