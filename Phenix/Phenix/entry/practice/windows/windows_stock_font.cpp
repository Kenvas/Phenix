#include "PrecompiledHeader.hpp"

#include "begin"

class StockFontWindow : public gui::Window
{
private:

    int iFont;
    int cxGrid, cyGrid;
    int wheel_delta;

protected:

    virtual HWND CreateWindowInstance() override
    {
        autox size   = GetSize();
        autox retval = CreateWindowEx
        (
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
            GetWindowClassName(),
            TEXT("Window"),
            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL,
            // 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, // x, y, width, height
            CW_USEDEFAULT, CW_USEDEFAULT, size.width, size.height,
            nullptr, // parent
            nullptr, // menu
            GetModuleHandle(nullptr),
            this // in message WM_NCCREATE, WM_CREATE, we can get 'this' pointer from 'lparam'
        );

        return retval;
    }

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();

        struct StockFontData
        {
            int idStockFont;
            TCHAR * const szStockFont;
        };
        StockFontData stockfont[] =
        {
            OEM_FIXED_FONT     , TEXT("OEM_FIXED_FONT"     ),
            ANSI_FIXED_FONT    , TEXT("ANSI_FIXED_FONT"    ),
            ANSI_VAR_FONT      , TEXT("ANSI_VAR_FONT"      ),
            SYSTEM_FONT        , TEXT("SYSTEM_FONT"        ),
            DEVICE_DEFAULT_FONT, TEXT("DEVICE_DEFAULT_FONT"),
            SYSTEM_FIXED_FONT  , TEXT("SYSTEM_FIXED_FONT"  ),
            DEFAULT_GUI_FONT   , TEXT("DEFAULT_GUI_FONT"   ),
        };

        autox cFonts = int(sizeof stockfont / sizeof stockfont[0]);
        autox hdc = HDC();
        autox ps = PAINTSTRUCT();
        autox tm = TEXTMETRIC();
        TCHAR szFaceName[LF_FACESIZE], szBuffer[LF_FACESIZE + 64];

        switch (message)
        {
        case WM_CREATE:
            SetScrollRange(hwnd, SB_VERT, 0, cFonts - 1, TRUE);
            return 0;
        case WM_DISPLAYCHANGE:
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        case WM_VSCROLL:
            switch (LOWORD(wparam))
            {
            case SB_TOP          : iFont = 0;              break;
            case SB_BOTTOM       : iFont = cFonts - 1;     break;
            case SB_LINEUP       :
            case SB_PAGEUP       : iFont--;                break;
            case SB_LINEDOWN     :
            case SB_PAGEDOWN     : iFont++;                break;
            case SB_THUMBPOSITION: iFont = HIWORD(wparam); break;
            }
            iFont = max(0, min(cFonts - 1, iFont));
            SetScrollPos(hwnd, SB_VERT, iFont, TRUE);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        case WM_KEYDOWN:
            switch (wparam)
            {
            case VK_HOME : SendMessage(hwnd, WM_VSCROLL, SB_TOP     , 0); break;
            case VK_END  : SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM  , 0); break;
            case VK_PRIOR:
            case VK_LEFT :
            case VK_UP   : SendMessage(hwnd, WM_VSCROLL, SB_LINEUP  , 0); break;
            case VK_NEXT :
            case VK_RIGHT:
            case VK_DOWN : SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0); break;
            }
            return 0;
        case WM_MOUSEWHEEL:
            wheel_delta = (short)HIWORD(wparam);
            if (wheel_delta < 0)
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
            else
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SelectObject(hdc, GetStockObject(stockfont[iFont].idStockFont));
            GetTextFace(hdc, LF_FACESIZE, szFaceName);
            GetTextMetrics(hdc, &tm);
            cxGrid = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
            cyGrid = tm.tmHeight + 3;
            TextOut(hdc, 300, 0, szBuffer, wsprintf(
                szBuffer,
                TEXT(" %s: Face Name = %s, CharSet = %i"),
                stockfont[iFont].szStockFont,
                szFaceName,
                tm.tmCharSet
            ));
            SetTextAlign(hdc, TA_TOP | TA_CENTER);

            for (int i = 0; i < 17; i++)
            {
                MoveToEx(hdc, (i + 2) * cxGrid,  2 * cyGrid, nullptr);
                LineTo  (hdc, (i + 2) * cxGrid, 19 * cyGrid);

                MoveToEx(hdc,      cxGrid, (i + 3) * cyGrid, nullptr);
                LineTo  (hdc, 18 * cxGrid, (i + 3) * cyGrid);
            }

            for (int i = 0; i < 16; i++)
            {
                TextOut(hdc, (2 * i + 5) * cxGrid / 2, 2 * cyGrid + 2, szBuffer, wsprintf(
                    szBuffer,
                    TEXT("%X-"),
                    i
                ));
                TextOut(hdc, 3 * cxGrid / 2, (i + 3) * cyGrid + 2, szBuffer, wsprintf(
                    szBuffer,
                    TEXT("-%X"),
                    i
                ));
            }

            for (int y = 0; y < 16; y++)
            {
                for (int x = 0; x < 16; x++)
                {
                    TextOut(hdc, (2 * x + 5) * cxGrid / 2, (y + 3) * cyGrid + 2, szBuffer, wsprintf(
                        szBuffer,
                        TEXT("%c"),
                        16 * x + y
                    ));
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        case WM_NCDESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<StockFontWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"