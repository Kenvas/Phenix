#include "phenix/entry/Entry.h"

// exit
FNX_AddGlobalEntry("exit")
{
    cout << lightfore << green << "wish you have a nice day, bye bye!" << reset << endl;
    Manager.Exit();
};