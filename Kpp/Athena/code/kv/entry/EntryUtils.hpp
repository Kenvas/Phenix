#pragma once

#include "kv/predef/Keyword.hpp"
#include "termcolor/termcolor.hpp"

#include <memory>
#include <string>
#include <vector>
#include <functional>

#define __KV_AutoVarName(x) __kv_auto_var_name__ ## x
#define _KV_AutoVarName(x) __KV_AutoVarName(x)
#define KV_AutoVarName _KV_AutoVarName(__COUNTER__)

#define KV_Entry_UsingNamespace \
    using namespace std; \
    using namespace termcolor; \
    using namespace kv::entry

#define KV_NewEntryItem(name) kv::entry::NewItem(name) += []( \
    kv::entry::CommandArgs const & args, \
    kv::entry::EntryManager & Manager)

#define KV_AddEntry(name) \
    KV_Entry_UsingNamespace; \
    static auto KV_AutoVarName = kv::entry::GetManager() \
        .GetEntry(std::get<0>(kv::entry::ExtractPathInfo(__FILE__))) += KV_NewEntryItem(name)

#define KV_AddGlobalEntry(name) \
    KV_Entry_UsingNamespace; \
    static auto KV_AutoVarName = kv::entry::GetManager() *= KV_NewEntryItem(name)

#define KV_QuickAddEntry KV_AddEntry(std::get<1>(kv::entry::ExtractPathInfo(__FILE__)))

#include "_namespace/begin"

class EntryObject;
class EntryManager;

using ObjectType   = EntryObject                                             ;
using StringType   = std::string                                             ;
using CommandArgs  = std::vector<StringType>                                 ;
using ActionType   = std::function<void(CommandArgs const &, EntryManager &)>;
using SharedType   = std::shared_ptr<ObjectType>                             ;
using SharedPtrs   = std::vector<SharedType>                                 ;
using PathInfoType = std::tuple<StringType, StringType>                      ;


SharedType NewItem(StringType const & name);
SharedType NewContainer(StringType const & name);

PathInfoType ExtractPathInfo(StringType const & fullpath);

SharedType operator+=(SharedType && entry, ActionType && action);
SharedType operator+=(SharedType && container, SharedType && entry);
SharedType operator*=(EntryManager & manager, SharedType && object);

EntryManager & GetManager();

#include "_namespace/end"