#pragma once

#include "wglext.h"

namespace fnx
{
    namespace app
    {
        namespace wgl
        {
            BOOL   WINAPI LoadExtenedFunctions();
            BOOL   WINAPI IsExtensionSupported(HDC hdc, LPCSTR extName);
            LPCSTR WINAPI GetExtensionsString (HDC hdc);
            HGLRC  WINAPI CreateContextAttribs(HDC hdc, HGLRC hShareContext, const int *attribList);
            BOOL   WINAPI ChoosePixelFormat   (HDC hdc, const int *piAttribIList, const float *pfAttribFList, int nMaxFormats, int *piFormats, int *nNumFormats);
            BOOL   WINAPI SwapInterval        (int interval);
            int    WINAPI GetSwapInterval     (void);
            HGLRC  WINAPI CreateContext       (HDC hdc);
            BOOL   WINAPI DeleteContext       (HGLRC hglrc);
            BOOL   WINAPI MakeCurrent         (HDC hdc, HGLRC hglrc);
        }
    }
}