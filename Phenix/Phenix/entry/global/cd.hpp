#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

// change directory
KV_AddGlobalEntry("cd")
{
    autox argc = args.size();
    if (argc == 2)
    {
        autox path = args[1];

        if (path == "#")
        {
            Manager.SetCurrent(Manager.GetRoot());
            Manager.SetCurrent(Manager.GetWorkspace());
            return;
        }

        Manager.SetCurrent(args[1]);
    }
    else
    {
        cout << yellow << "usage: cd <path>" << reset << endl;
    }
};