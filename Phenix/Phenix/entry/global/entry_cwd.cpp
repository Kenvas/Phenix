#include "phenix/entry/Entry.h"

// change work directory
FNX_AddGlobalEntry("cwd")
{
    autox argc = args.size();

    if (argc > 1)
    {
        Manager.SetWorkspace(args[1]);
    }

    cout << cyan << "@ " << Manager.GetWorkspace() << reset << endl;
};