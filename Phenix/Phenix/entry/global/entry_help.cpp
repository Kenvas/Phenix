#include "phenix/entry/Entry.hpp"

// show available commands
FNX_AddGlobalEntry("help")
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