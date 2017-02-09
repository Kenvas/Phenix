#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

// new console
KV_AddGlobalEntry("cmd")
{
    autox bootpath = Manager.GetBootPath();
    autox current  = Manager.GetCurrent();
    autox command  = StringType();
    command.append("start")
        .append(" ").append(bootpath)
        .append(" ").append(current->GetFullName());
    system(command.c_str());
};