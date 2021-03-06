#include "PrecompiledHeader.hpp"

#include "../IncludeAll.hpp"

#include <filesystem>

using namespace std::experimental::filesystem;

#include "../_namespace/begin"

SharedType NewItem(StringType const & name)
{
    return std::make_shared<EntryObject>(name, EntryType::Item);
}

SharedType NewContainer(StringType const & name)
{
    return std::make_shared<EntryObject>(name, EntryType::Container);
}

PathInfoType ExtractPathInfo(StringType const & fullpath)
{
    autox p = path(fullpath);
    //autox pos1 = fullpath.rfind('\\');
    //autox pos2 = fullpath.rfind('.');
    //autox lhs  = fullpath.substr(0, pos1 + 1);
    //autox rhs  = fullpath.substr(pos1 + 1, pos2 - pos1 - 1);
    autox folder = p.parent_path();
    autox name = p.stem();
    return PathInfoType(folder.string(), name.string());
}

SharedType operator+=(SharedType && entry, ActionType && action)
{
    return entry->SetAction(action);
}

SharedType operator+=(SharedType && container, SharedType && entry)
{
    return container->AddChild(entry);
}

SharedType operator*=(EntryManager & manager, SharedType && object)
{
    return manager.AddGlobalEntry(object);
}

EntryManager & GetManager()
{
    static autox manager_pointer = std::shared_ptr<EntryManager>(nullptr);
    if (manager_pointer == nullptr)
        manager_pointer = std::make_shared<EntryManager>();
    return *manager_pointer;
}

#include "../_namespace/end"