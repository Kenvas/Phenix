#include "PrecompiledHeader.hpp"
#include "global/IncludeAll.hpp"
#include "kv/entry/IncludeAll.hpp"

using namespace std;
using namespace termcolor;
using namespace kv::entry;

int main(int argc, char ** argv)
{
    autox manager = GetManager();
    autox path    = ExtractPathInfo(argv[0]);

    cout << "boot path: " << argv[0] << endl;
    cout << "work directory: " << get<0>(path).data() << endl;
    SetCurrentDirectoryA(get<0>(path).data());
    set_title(get<1>(path));

    manager.SetWorkspace(__FILE__);
    manager.SetBootPath(argv[0]);

    _putenv((string("PWD=") + get<0>(path)).c_str());

    switch (argc)
    {
    default:
    case 1:
        manager.SetCurrent(get<0>(ExtractPathInfo(__FILE__)) + "/../");
        break;
    case 2:
        manager.SetCurrent(argv[1]);
        break;
    }

    manager.Run();

    return 0;
}