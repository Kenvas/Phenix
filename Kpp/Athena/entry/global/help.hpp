#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

// show available commands
KV_AddGlobalEntry("help")
{
    autox global  = Manager.GetGlobal();
    autox entries = global->GetChidren();

    cout << yellow << "support commands:" << green << endl;
    for (autox entry : entries)
    {
        cout << "    " << entry->GetName() << endl;
    }
    cout << reset;
};