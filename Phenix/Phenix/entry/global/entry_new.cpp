#include "phenix/entry/Entry.hpp"

// new console
FNX_AddGlobalEntry("new")
{
    autox bootpath = Manager.GetBootPath();
    autox current  = Manager.GetCurrent();
    autox command  = StringType();
    command.append("start")
        .append(" ").append(bootpath)
        .append(" ").append(current->GetFullName());
    system(command.c_str());
};