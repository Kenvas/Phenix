#include "phenix/entry/Entry.h"

using namespace std;
using namespace termcolor;
using namespace fnx::entry;


int main(int argc, char ** argv)
{
    set_title(get<1>(ExtractPathInfo(argv[0])));

    autox mgr = GetManager();

    mgr.SetWorkspace(__FILE__);

    mgr.SetBootPath(argv[0]);

    if (argc > 1)
    {
        mgr.SetCurrent(argv[1]);
    }

    mgr.Run();

    return 0;
}