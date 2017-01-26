#include "PrecompiledHeader.hpp"

#include "../EntryObject.hpp"

using namespace std;

#include "../_namespace/begin"

EntryObject::EntryObject(StringType const & name, EntryType const type)
    : Type_(type)
    , Name_(name)
    , Action_(nullptr)
    , Parent_(nullptr)
    , Children_(0)
{

}

EntryType const EntryObject::GetType() const
{
    return Type_;
}

StringType const & EntryObject::GetName() const
{
    return Name_;
}

StringType const EntryObject::GetFullName() const
{
    autox names  = vector<StringType const *>();
    autox object = shared_from_this();
    autox length = (size_t)0;
    while (object != nullptr)
    {
        autox name = object->GetName();
        names.push_back(&name);
        length += name.length();
        autox parent = object->GetParent();
        object = (object != parent) ? parent : nullptr;
        if (object == nullptr)
            names.pop_back();
    }
    autox count  = names.size();
    autox retval = StringType();
    retval.reserve(length + count + 1);
    for (autox i = (int)(count - 1); i >= 0; i--)
        retval.append("/").append(*names[i]);
    return retval;
}

SharedType const & EntryObject::GetParent() const
{
    return Parent_;
}

void EntryObject::SetParent(SharedType const & parent)
{
    Parent_ = parent;
}

SharedPtrs const & EntryObject::GetChidren() const
{
    return Children_;
}

SharedType EntryObject::SetAction(ActionType const & action)
{
    Action_ = action;
    return shared_from_this();
}

SharedType EntryObject::AddChild(SharedType & child)
{
    autox retval = shared_from_this();
    child->SetParent(retval);
    Children_.push_back(child);
    return retval;
}

SharedType EntryObject::AddChildren(initializer_list<SharedType> list)
{
    for (autox obj : list)
        Children_.push_back(obj);
    return shared_from_this();
}

void EntryObject::Execute(CommandArgs const & args) const
{
    if (Action_ != nullptr)
        Action_(args, GetManager());
}

void EntryObject::Execute() const
{
    Execute(CommandArgs());
}

#include "../_namespace/end"