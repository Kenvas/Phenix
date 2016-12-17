#include "phenix/entry/Entry.h"

// list current directory's entries
FNX_AddGlobalEntry("ls")
{
    autox current = Manager.GetCurrent();
    autox entries = current->GetChidren();

    cout << yellow << "included objects:" << endl;
    for (autox entry : entries)
    {
        cout << ((entry->GetType() == EntryType::Item) ? green : cyan)
            << "    " << entry->GetName()
            << endl;
    }
    cout << reset;
};