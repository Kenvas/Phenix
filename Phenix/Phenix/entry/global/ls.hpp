#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

// list current directory's entries
KV_AddGlobalEntry("ls")
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