#include "phenix/entry/Entry.hpp"

#include "phenix/app/OpenGLApp.hpp"

using namespace std;
using namespace fnx::app;

FNX_QuickAddEntry
{
    autox app = make_shared<OpenGLApp>(800,600);
    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};