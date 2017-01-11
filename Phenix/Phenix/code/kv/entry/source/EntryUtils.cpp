#include "PrecompiledHeader.hpp"
#include "kv/entry/EntryUtils.hpp"

namespace kv
{
    namespace entry
    {
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
            autox pos1 = fullpath.rfind('\\');
            autox pos2 = fullpath.rfind('.');
            autox lhs  = fullpath.substr(0, pos1 + 1);
            autox rhs  = fullpath.substr(pos1 + 1, pos2 - pos1 - 1);
            return PathInfoType(lhs, rhs);
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
    }
}