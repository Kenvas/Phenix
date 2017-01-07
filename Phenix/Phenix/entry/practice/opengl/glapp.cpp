#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

#include "kv/gui/OpenGLWindow.hpp"

using namespace std;
using namespace kv::gui;

KV_QuickAddEntry
{
    autox app = make_shared<OpenGLWindow>(800,600);
    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};