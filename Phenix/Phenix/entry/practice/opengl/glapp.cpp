#include "phenix/entry/Entry.h"

#include "phenix/app/OpenGLApp.h"

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