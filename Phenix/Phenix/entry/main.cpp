#include "PrecompiledHeader.hpp"
#include "phenix/entry/Entry.hpp"

#include "fmt/format.h"

#if (4 > 3)
using namespace std;
using namespace termcolor;
using namespace fnx::entry;
#endif

int main(int argc, char ** argv)
{
    cout << fmt::format("__cplusplus: {0}", __cplusplus) << endl;
    cout << fmt::format("_MSC_VER: {0}", _MSC_VER) << endl;
    cout << fmt::format("_MSC_FULL_VER: {0}", _MSC_FULL_VER) << endl;
    //cout << fmt::format("__clang__: {0}", __clang__) << endl;
    //cout << fmt::format("__clang_major__: {0}", __clang_major__) << endl;
    //cout << fmt::format("__clang_minor__: {0}", __clang_minor__) << endl;
    //cout << fmt::format("__clang_patchlevel__: {0}", __clang_patchlevel__) << endl;
    //cout << fmt::format("__SSE__: {0}", __SSE__) << endl;

    autox mgr  = GetManager();
    autox path = ExtractPathInfo(argv[0]);

    set_title(get<1>(path));

    mgr.SetWorkspace(__FILE__);
    mgr.SetBootPath(argv[0]);

    _putenv((string("PWD=") + get<0>(path)).c_str());

    switch (argc)
    {
    default:
    case 1:
        mgr.SetCurrent(get<0>(ExtractPathInfo(__FILE__)));
        break;
    case 2:
        mgr.SetCurrent(argv[1]);
        break;
    }

    mgr.Run();

    return 0;
}