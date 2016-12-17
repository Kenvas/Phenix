#include "phenix/entry/Entry.h"

FNX_Entry_UsingNamespace;

static void Build(int depth, int trunk, SharedType const & node)
{
    for (autox i = 0; i < depth; i++)
    {
        autox has_flag = ( trunk & ( 1 << i ) );
        cout << yellow
            << (has_flag ? '|' : ' ')
            << "  ";
    }

    if (depth > 0)
    {
        cout << "\b\b\b" << "|--";
    }

    cout << ( (node->GetType() == EntryType::Item) ? green : cyan )
        << node->GetName() << endl;

    autox children = node->GetChidren();
    autox count    = children.size();
    for (autox i = 0; i < count; i++)
    {
        autox child = children[i];
        autox flag  = (i < (count - 1)) ? (1 << depth) : 0;
        Build(depth + 1, trunk | flag, child);
    }
}

// recursive list entries
FNX_AddGlobalEntry("tree")
{
    autox depth   = 0;
    autox trunk   = 0;
    autox current = Manager.GetCurrent();
    Build(depth, trunk, current);
    cout << reset;
};
