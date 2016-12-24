#include "PrecompiledHeader.hpp"
#include "phenix/entry/Entry.hpp"

// environment
FNX_AddGlobalEntry("env")
{
    autox argc = args.size();
    switch (argc)
    {
    case 2:
    {
        autox name = args[1];
        autox reqsz = size_t(0);
        char buffer[2048];
        getenv_s(&reqsz, buffer, name.c_str());
        cout << cyan << buffer << reset << endl;
        break;
    }
    case 3:
    {
        break;
    }
    default:
        cout << yellow << "usage: env <name> [<value>]" << reset << endl;
        break;
    }
};