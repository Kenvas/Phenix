#pragma once

#include <minwindef.h>

#include "kv/predef/Char.hpp"

#include "_namespace/begin"
namespace utils
{
bool PrintErrorMessageInfo() noexcept;
bool RegisterWindowClass(PCTSTR const class_name, WNDPROC const window_procdure) noexcept;
void UnregisterWindowClass(PCTSTR const class_name) noexcept;
void PrintWindowMessageInfo(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) noexcept;
cchar8s GetWindowMessageName(size_t const message) noexcept;
}
#include "_namespace/end"