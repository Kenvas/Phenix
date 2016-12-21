#include "phenix/entry/Entry.h"

#include <iostream>

using namespace std;
using namespace termcolor;

namespace fnx
{
    namespace entry
    {
        inline namespace detail
        {
            static VectorArgs _ParseInput(StringType const & input)
            {
                autox p = input.c_str();
                autox q = p;
                autox eof    = (*q == 0);
                autox quoted = false;
                autox retval = VectorArgs();

                while (!eof)
                {
                    switch (*q)
                    {
                    case 0:
                        if (quoted)
                        {
                            cout << red << "error: quote not matched." << reset << endl;
                            retval.clear();
                        }
                        else if (p != q)
                        {
                            retval.push_back(StringType(p, q));
                        }
                        eof = true;
                        break;
                    case '\"':
                        if (quoted)
                        {
                            retval.push_back(StringType(p, q));
                        }
                        ++q;
                        p = q;
                        quoted = !quoted;
                        break;
                    case ' ':
                    case '\t':
                        if (!quoted && (p != q))
                        {
                            retval.push_back(StringType(p, q));
                        }
                        ++q;
                        p = quoted ? p : q;
                        break;
                    default:
                        ++q;
                        break;
                    }
                }

                return retval;
            }

            static VectorArgs _SplitPath(StringType const & input)
            {
                autox p = input.c_str();
                autox q = p;
                autox eof = (*q == 0);
                autox ret = VectorArgs();

                while (!eof)
                {
                    switch (*q)
                    {
                    case 0:
                        eof = true;
                    case '/':
                    case '\\':
                        if (p != q)
                        {
                            ret.push_back(StringType(p, q));
                        }
                        ++q;
                        p = q;
                        break;
                    default:
                        ++q;
                        break;
                    }
                }

                return ret;
            }

            static SharedType _AddEntry(
                SharedType & node,
                SharedType & entry)
            {
                node->AddChild(entry);
                return entry;
            }

            static SharedType const * _FindObject(
                SharedType const & node,
                StringType const & name)
            {
                if (name == ".")
                {
                    return &node;
                }

                if (name == "..")
                {
                    return &node->GetParent();
                }

                autox children = node->GetChidren();
                for (autox child : children)
                {
                    if (child->GetName() == name)
                        return &child;
                }

                return nullptr;
            }

            static bool const _FuzzyMatch(StringType const & name, StringType const & input)
            {
                if (name.size() < input.size())
                    return false;
                autox p = name.c_str();
                autox q = input.c_str();
                while (*q != 0)
                {
                    while ((*p != 0) && (*p != *q)) { ++p; }
                    if ((*p == 0) && (*q != 0)) { return false; }
                    ++q;
                }
                return true;
            }

            static vector<SharedType const *> _FuzzyFindObject(
                SharedType const & node,
                StringType const & name,
                bool try_strict_find = false)
            {
                autox retval = vector<SharedType const *>();
                autox object = try_strict_find ? _FindObject(node, name) : nullptr;

                if (object != nullptr)
                {
                    retval.push_back(object);
                }
                else
                {
                    autox children = node->GetChidren();
                    for (autox child : children)
                    {
                        autox s = child->GetName();
                        if (_FuzzyMatch(s, name))
                            retval.push_back(&child);
                    }
                }

                return retval;
            }

            static vector<SharedType const *> _FindObjectRecursive(
                SharedType const & node,
                StringType const & name)
            {
                autox retval  = vector<SharedType const *>();
                autox nodes   = vector<SharedType const *>();
                nodes.push_back(&node);

                while (nodes.size() > 0)
                {
                    autox p = *nodes.back();
                    autox s = p->GetName();
                    nodes.pop_back();
                    if ((p != node) && _FuzzyMatch(s, name))
                    {
                        retval.push_back(&p);
                    }
                    else
                    {
                        autox children = p->GetChidren();
                        for (autox child : children)
                            nodes.push_back(&child);
                    }
                }

                return retval;
            }

            static bool _ExecuteCommand(
                SharedType const & entry,
                VectorArgs const & args)
            {
                switch (entry->GetType())
                {
                case EntryType::Item:
                    entry->Execute(args);
                    break;
                case EntryType::Container:
                    GetManager().SetCurrent(entry);
                    break;
                default:
                    cout << red << "error: get an object with 'Unknown' type." << reset << endl;
                    break;
                }
                return true;
            }

            static bool _TryExecuteCommand(
                SharedType const & node,
                VectorArgs const & args)
            {
                autox command = args[0];
                autox pEntry  = _FindObject(node, command);
                if (pEntry == nullptr)
                    return false;
                _ExecuteCommand(*pEntry, args);
                return true;
            }

            static bool _FuzzyTryExecuteCommand(
                SharedType const & node,
                VectorArgs const & args,
                bool auto_exec)
            {
                autox command  = args[0];
                autox pEntries = _FuzzyFindObject(node, command);
                autox count    = pEntries.size();

                if (count == 0)
                    return false;

                if (count == 1 && auto_exec)
                {
                    autox entry = *pEntries[0];
                    _ExecuteCommand(entry, args);
                }
                else
                {
                    autox is_global = (GetManager().GetGlobal() == node);
                    autox message = StringType();
                    message.append("do you mean");
                    if (is_global)
                        message.append("[global]: ");
                    else
                        message.append("[normal]: ");

                    cout << yellow << message << endl;
                    for (autox pEntry : pEntries)
                    {
                        autox entry = *pEntry;
                        autox is_item = (entry->GetType() == EntryType::Item);
                        cout << (is_item ? green : cyan)
                            << "    " << entry->GetName() << endl;
                    }
                    cout << reset;
                }

                return true;
            }

            static bool _TryCallEntry(
                SharedType const & node,
                VectorArgs const & args)
            {
                autox names    = _SplitPath(args[0]);
                autox entries  = vector<SharedType const *>();
                autox children = vector<SharedType const *>();

                entries.push_back(&node);

                for (autox name : names)
                {
                    children.clear();
                    for (autox entry : entries)
                    {
                        autox retvals = _FindObjectRecursive(*entry, name);
                        for (autox v : retvals)
                            children.push_back(v);
                    }
                    entries = children;
                }

                autox found_count = entries.size();
                if (found_count == 0) return false;

                if (found_count == 1)
                {
                    autox entry = *entries[0];
                    autox command = const_cast<StringType &>(args[0]);
                    command = entry->GetName();
                    _ExecuteCommand(entry, args);
                }
                else
                {

                }

                return true;
            }

            static StringType const _GetPromptName(SharedPtrs const & objects)
            {
                autox length = objects.size();
                for (autox o : objects)
                    length += o->GetName().length() + 1;
                autox retval = StringType();
                retval.reserve(length);
                retval.append("#/");
                for (autox o : objects)
                    retval.append(o->GetName()).push_back('/');
                retval.pop_back();
                retval.append(" > ");
                return retval;
            }

            enum ContextStatus : size_t
            {
                Run = 1,
            };
        }

        EntryManager::EntryManager()
            : Root_(NewContainer("~"))
            , Global_(NewContainer("<Global>"))
            , Workspace_(__FILE__)
            , CurrentNodes_(SharedPtrs())
            , ContextFlag_(ContextStatus::Run)
        {
            Root_->SetParent(Root_);
            Global_->SetParent(Global_);
        }

        SharedType EntryManager::GetEntry(StringType const & path)
        {
            //if (path.find(Workspace_) != 0)
            //{
            //    xauto message = StringType();
            //    message.append("error: path = '").append(path)
            //        .append("' is not under workspace = '").append(Workspace_)
            //        .append("'.");
            //    cout << red << message << reset << endl;
            //    return Root_;
            //}

            //xauto relpath = path.substr(Workspace_.length());
            autox info   = ExtractPathInfo(path);
            autox folder = get<0>(info);
            //autox file   = get<1>(info);
            autox names  = _SplitPath(folder);
            autox node   = Root_;

            for (autox name : names)
            {
                autox p = _FindObject(node, name);
                if (p == nullptr)
                {
                    autox c = NewContainer(name);
                    node->AddChild(c);
                    p = &c;
                }
                node = *p;
            }

            //xauto f = NewItem(file);
            //node->AddChild(f);

            return node;
        }

        SharedType EntryManager::AddGlobalEntry(SharedType & entry)
        {
            return _AddEntry(Global_, entry);
        }

        SharedType const & EntryManager::GetGlobal() const
        {
            return Global_;
        }

        StringType const & EntryManager::GetBootPath() const
        {
            return BootPath_;
        }

        void EntryManager::SetBootPath(StringType const & path)
        {
            BootPath_ = path;
        }

        void EntryManager::SetWorkspace(StringType const & path)
        {
            autox info = ExtractPathInfo(path);
            Workspace_ = get<0>(info);
        }

        StringType const & EntryManager::GetWorkspace() const
        {
            return Workspace_;
        }

        SharedType const & EntryManager::GetRoot() const
        {
            return Root_;
        }

        SharedType const & EntryManager::GetCurrent() const
        {
            return (CurrentNodes_.size() > 0)
                ? CurrentNodes_.back()
                : Root_;
        }

        void EntryManager::SetCurrent(StringType const & path)
        {
            autox names   = _SplitPath(path);
            autox current = (SharedType)GetCurrent();
            for (autox name : names)
            {
                autox strict = true;
                autox nodes  = _FuzzyFindObject(current, name, strict);
                autox count  = nodes.size();
                if (count != 1)
                {
                    autox message = StringType();
                    message.append("error: not found path = ");
                    for (autox name : names)
                        message.append(name).push_back('/');
                    message.pop_back();
                    cout << red << message << reset << endl;
                    break;
                }
                autox node = *nodes[0];
                if (node->GetType() == EntryType::Item)
                {
                    break;
                }
                current = node;
            }
            SetCurrent(current);
        }

        void EntryManager::SetCurrent(SharedType const & current)
        {
            CurrentNodes_.clear();
            autox p = (SharedType)current;
            while (p != Root_)
            {
                autox pos = CurrentNodes_.cbegin();
                CurrentNodes_.insert(pos, p);
                p = p->GetParent();
            }
        }

        void EntryManager::Run()
        {
            autox input = StringType();
            while (ContextFlag_ & ContextStatus::Run)
            {
                autox prompt = _GetPromptName(CurrentNodes_);
                //cout << magenta << "#"
                //    << cyan << prompt
                //    << reset << " > ";
                cout << prompt << endl << "$ ";
                //cout << "$ ";

                getline(cin, input);
                cin.clear();

                autox args = _ParseInput(input);
                if (args.size() == 0) continue;
                autox command   = args[0];
                autox current   = GetCurrent();
                autox auto_exec = false;

                if (command.size() > 1)
                {
                    switch (command[0])
                    {
                    case ':':
                        auto_exec = true;
                        command = command.substr(1);
                        break;
                    case '/':
                        command = command.substr(1);
                        if (_TryCallEntry(current, args))
                            continue;
                        break;
                    default:
                        break;
                    }
                }

                if (_TryExecuteCommand(current, args))
                    continue;
                if (_TryExecuteCommand(Global_, args))
                    continue;

                autox has_suggestion = false;
                has_suggestion |= _FuzzyTryExecuteCommand(current, args, auto_exec);
                if (has_suggestion && auto_exec)
                    continue;
                has_suggestion |= _FuzzyTryExecuteCommand(Global_, args, auto_exec);
                if (has_suggestion && auto_exec)
                    continue;

                if (!has_suggestion)
                    cout << red << "not found command: " << green << command << white << endl;
            }
            return;
        }

        void EntryManager::Exit()
        {
            ContextFlag_ &= ~ContextStatus::Run;
        }
    }
}