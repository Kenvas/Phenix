#include "phenix/entry/Entry.h"

using namespace std;
using namespace termcolor;
using namespace fnx::entry;

int main(int argc, char ** argv)
{
    autox mgr  = GetManager();
    autox path = ExtractPathInfo(argv[0]);

    set_title(get<1>(path));

    mgr.SetWorkspace(__FILE__);
    mgr.SetBootPath(argv[0]);
    mgr.SetCurrent(get<0>(ExtractPathInfo(__FILE__)));

    _putenv((string("PWD=") + get<0>(path)).c_str());

    if (argc > 1)
    {
        mgr.SetCurrent(argv[1]);
    }

    mgr.Run();

    return 0;
}