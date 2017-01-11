#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

#include "kv/predef/keyword/inline.hpp"

kvinline(off) void foo(int x)
{
    return;
}

KV_QuickAddEntry
{
    return 0;
};
