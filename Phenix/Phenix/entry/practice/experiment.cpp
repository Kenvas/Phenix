#include "phenix/entry/Entry.h"

#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl/GL.h>

FNX_QuickAddEntry
{
    //autox hwnd    = GetConsoleWindow();
    //autox hdc     = GetDC(hwnd);
    //autox pfd     = PIXELFORMATDESCRIPTOR{ 0 };
    //autox error   = SetPixelFormat(hdc, 1, &pfd);
    //autox hglrc   = wglCreateContext(hdc);
    //wglMakeCurrent(hdc, hglrc);
    //autox version = glGetString(GL_VERSION);
    //autox vendor  = glGetString(GL_VENDOR);
    //wglMakeCurrent(nullptr, nullptr);
    //wglDeleteContext(hglrc);

    //TCHAR className[128];
    //autox name = GetClassName(hwnd, className, sizeof(className) / sizeof(TCHAR));
};