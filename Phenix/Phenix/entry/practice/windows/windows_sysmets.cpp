#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"
#include "kv/native/windows/utils.hpp"

#include "kv/gui/Window.hpp"
#include "include/SysMets.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

using namespace kv;
using namespace kv::native::windows;

class SysMetsWindow : public gui::Window
{
protected:

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        static int cxChar, cxCaps, cyChar;
        HDC hdc;
        PAINTSTRUCT ps;
        TCHAR szBuffer[10];
        TEXTMETRIC tm;

        autox window_handle = GetWindowHandle();

        switch (message)
        {
        case WM_CREATE:
            hdc = GetDC(window_handle);
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = ((tm.tmPitchAndFamily & 1) ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(window_handle, hdc);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(window_handle, &ps);
            for (int i = 0; i < NUMLINES; i++)
            {
                autox label = sysmetrics[i].szLabel;
                TextOut(hdc, 0, cyChar * i, label, lstrlen(label));

                autox desc = sysmetrics[i].szDesc;
                TextOut(hdc, 22 * cxCaps, cyChar * i, desc, lstrlen(desc));

                SetTextAlign(hdc, TA_RIGHT | TA_TOP);

                TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(window_handle, &ps);
            return 0;
        case WM_NCDESTROY:
            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(window_handle, message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<SysMetsWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};