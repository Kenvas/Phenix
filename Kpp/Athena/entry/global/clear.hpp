#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

// clear screen
KV_AddGlobalEntry("clear")
{
    system("cls");
};