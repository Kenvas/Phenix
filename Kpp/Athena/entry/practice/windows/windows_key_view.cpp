#include "PrecompiledHeader.hpp"

#define KV_KEYVIEW_VERSION 2

#include "begin"

class KeyViewWindow : public gui::Window
{
private:

    DWORD dwCharSet = DEFAULT_CHARSET;
    int cxClientMax, cxClient, cxChar;
    int cyClientMax, cyClient, cyChar;
    int cLinesMax, cLines;
    PMSG pmsg;
    RECT rectScroll;
    HFONT hfont;
    TCHAR * const szTop = TEXT("Message        Key       Char      Repeat Scan Ext ALT Prev Tran");
    TCHAR * const szUnd = TEXT("_______        ___       ____      ______ ____ ___ ___ ____ ____");
    TCHAR * const szFormat[2] =
    {
        TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
        TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s"),
    };
    TCHAR * const szYes  = TEXT("Yes");
    TCHAR * const szNo   = TEXT("No");
    TCHAR * const szDown = TEXT("Down");
    TCHAR * const szUp   = TEXT("Up");

protected:

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();

        TCHAR * const szMessage[] =
        {
            TEXT("WM_KEYDOWN"),
            TEXT("WM_KEYUP"),
            TEXT("WM_CHAR"),
            TEXT("WM_DEADCHAR"),
            TEXT("WM_SYSKEYDOWN"),
            TEXT("WM_SYSKEYUP"),
            TEXT("WM_SYSCHAR"),
            TEXT("WM_SYSDEADCHAR"),
        };

        autox hdc = HDC();
        autox ps = PAINTSTRUCT();
        autox tm = TEXTMETRIC();
        TCHAR szBuffer[128], szKeyName[32];

        switch (message)
        {
#if KV_KEYVIEW_VERSION >= 2
        case WM_INPUTLANGCHANGE:
            dwCharSet = static_cast<DWORD>(wparam);
            // fall through
#endif
        case WM_CREATE:
        case WM_DISPLAYCHANGE:
            cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
            cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);
            hdc         = GetDC(hwnd);
#if KV_KEYVIEW_VERSION >= 2
            if (hfont != nullptr) DeleteObject(hfont);
            hfont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0
                , dwCharSet, 0, 0, 0, FIXED_PITCH, nullptr);
            SelectObject(hdc, hfont);
#else
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
#endif
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight;
            ReleaseDC(hwnd, hdc);
            if (pmsg) free(pmsg);
            cLinesMax = cyClientMax / cyChar;
            pmsg      = (PMSG)malloc(cLinesMax * sizeof(MSG));
            cLines    = 0;
            // fall through
        case WM_SIZE:
            if (message == WM_SIZE)
            {
                cxClient = LOWORD(lparam);
                cyClient = HIWORD(lparam);
            }
            rectScroll.left   = 0;
            rectScroll.right  = cxClient;
            rectScroll.top    = cyChar;
            rectScroll.bottom = cyChar * (cyClient / cyChar);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_DEADCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSCHAR:
        case WM_SYSDEADCHAR:
            // rearrange storage array
            for (int i = cLinesMax - 1; i > 0; i--)
            {
                pmsg[i] = pmsg[i - 1];
            }
            pmsg[0].hwnd    = hwnd;
            pmsg[0].message = message;
            pmsg[0].wParam  = wparam;
            pmsg[0].lParam  = lparam;

            cLines = min(cLines + 1, cLinesMax);
            // scroll up th display
            ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);
            break; // i.e., call DefWindowProc so sys messages work

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
            TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));
            for (int i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
            {
                autox m = pmsg[i].message;
                int iType = false
                    || (m == WM_CHAR)
                    || (m == WM_SYSCHAR)
                    || (m == WM_DEADCHAR)
                    || (m == WM_SYSDEADCHAR);
                GetKeyNameText((LONG)pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
                TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
                    wsprintf(
                        szBuffer,
                        szFormat[iType],
                        szMessage[pmsg[i].message - WM_KEYFIRST],
                        pmsg[i].wParam,
                        (PTSTR)(iType ? TEXT(" ") : szKeyName),
                        (TCHAR)(iType ? pmsg[i].wParam : ' '),
                        LOWORD(pmsg[i].lParam),
                        HIWORD(pmsg[i].lParam) & 0xff,
                        (0x01000000 & pmsg[i].lParam) ? szYes : szNo,
                        (0x20000000 & pmsg[i].lParam) ? szYes : szNo,
                        (0x40000000 & pmsg[i].lParam) ? szDown : szUp,
                        (0x80000000 & pmsg[i].lParam) ? szUp : szDown
                    ));
            }
            EndPaint(hwnd, &ps);
            return 0;
        case WM_NCDESTROY:
            if (pmsg) free(pmsg);
            if (hfont != nullptr) DeleteObject(hfont);
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<KeyViewWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"