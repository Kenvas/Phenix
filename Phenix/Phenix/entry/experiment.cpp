#include "PrecompiledHeader.hpp"

#include "kv/entry/IncludeAll.hpp"
#include "kv/predef/IncludeAll.hpp"

#include "kv/common/BaseType.hpp"

#include "kv/native/windows/utils.hpp"
#include "kv/native/windows/wgl.hpp"

KV_QuickAddEntry
{
    auto b = wglGetProcAddress("hello");
    kv::native::windows::utils::PrintErrorInfo();
    return 0;
};