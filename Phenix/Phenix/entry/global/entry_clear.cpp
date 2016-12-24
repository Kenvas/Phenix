#include "PrecompiledHeader.hpp"
#include "phenix/entry/Entry.hpp"

// clear screen
FNX_AddGlobalEntry("clear")
{
    system("cls");
};