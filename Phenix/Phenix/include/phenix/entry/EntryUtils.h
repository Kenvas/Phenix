#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "termcolor/termcolor.hpp"

#include "phenix/Predef.h"

#define FNX_Entry_UsingNamespace \
    using namespace std; \
    using namespace termcolor; \
    using namespace fnx::entry

#define FNX_NewEntryItem(name) fnx::entry::NewItem(name) += []( \
    fnx::entry::VectorArgs const & args, \
    fnx::entry::EntryManager & Manager)

#define FNX_AddEntry(name) \
    FNX_Entry_UsingNamespace; \
    static auto retval = fnx::entry::GetManager() \
        .GetEntry(std::get<0>(fnx::entry::ExtractPathInfo(__FILE__))) += FNX_NewEntryItem(name)

#define FNX_AddGlobalEntry(name) \
    FNX_Entry_UsingNamespace; \
    static auto retval = fnx::entry::GetManager() *= FNX_NewEntryItem(name)

#define FNX_QuickAddEntry FNX_AddEntry(std::get<1>(fnx::entry::ExtractPathInfo(__FILE__)))

namespace fnx
{
    namespace entry
    {
        class EntryObject;
        class EntryManager;

        _alias_(ObjectType  , EntryObject                                            );
        _alias_(StringType  , std::string                                            );
        _alias_(VectorArgs  , std::vector<StringType>                                );
        _alias_(ActionType  , std::function<void(VectorArgs const &, EntryManager &)>);
        _alias_(SharedType  , std::shared_ptr<ObjectType>                            );
        _alias_(SharedPtrs  , std::vector<SharedType>                                );
        _alias_(PathInfoType, std::tuple<StringType, StringType>                     );


        SharedType NewItem(StringType const & name);
        SharedType NewContainer(StringType const & name);

        PathInfoType ExtractPathInfo(StringType const & fullpath);

        SharedType operator+=(SharedType && entry, ActionType && action);
        SharedType operator+=(SharedType && container, SharedType && entry);
        SharedType operator*=(EntryManager & manager, SharedType && object);

        EntryManager & GetManager();
    }
}