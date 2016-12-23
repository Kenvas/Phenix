#include "phenix/entry/Entry.hpp"

// exit
FNX_AddGlobalEntry("exit")
{
    cout << lightfore << green << "wish you have a nice day, bye bye!" << reset << endl;
    Manager.Exit();
};