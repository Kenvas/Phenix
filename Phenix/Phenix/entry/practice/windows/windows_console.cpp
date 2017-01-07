#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define BUFFER(x, y) *(pBuffer + y * cxBuffer + x)

KV_QuickAddEntry
{
    TCHAR szAppName[] = TEXT("Typer");

    auto hInstance = GetModuleHandle(nullptr);
    auto wndclass = WNDCLASS{0};
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.hInstance     = hInstance;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    auto hwnd = CreateWindow(szAppName,           // window class name
                             TEXT("Typer"),       // window caption
                             WS_OVERLAPPEDWINDOW, // window style
                             CW_USEDEFAULT,       // initial x position
                             CW_USEDEFAULT,       // initial y position
                             CW_USEDEFAULT,       // initial x size
                             CW_USEDEFAULT,       // initial y size
                             NULL,                // parent window handle
                             NULL,                // window menu handle
                             hInstance,           // program instance handle
                             NULL);               // creation parameters

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    auto msg = MSG();
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
};

/*----------------------------------------------------------------------------
    typer.cpp -- Typing Program
----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
    keyView.cpp -- Displays keyboard and character messages
----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
    SysMets4.cpp -- System Metrics Display Program ver4 use keyboard
 *----------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DWORD dwCharSet = DEFAULT_CHARSET;
    static int cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
    static TCHAR *pBuffer = NULL;

    int x, y, i;
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    switch (message)
    {
    case WM_INPUTLANGCHANGE:
        dwCharSet = (DWORD)wParam;

    case WM_CREATE:
        hdc = GetDC(hwnd);

        SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas")));
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight;

        DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
        ReleaseDC(hwnd, hdc);

    case WM_SIZE:
        if (message == WM_SIZE)
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
        }

        cxBuffer = max(1, cxClient / cxChar);
        cyBuffer = max(1, cyClient / cyChar);

        if (pBuffer != NULL)
            free(pBuffer);

        pBuffer = (TCHAR *)malloc(sizeof(TCHAR) * cxBuffer * cyBuffer);

        for (y = 0; y < cyBuffer; y++)
            for (x = 0; x < cxBuffer; x++)
                BUFFER(x, y) = ' ';

        xCaret = 0;
        yCaret = 0;

        if (hwnd == GetFocus())
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);

        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_SETFOCUS:
        CreateCaret(hwnd, NULL, cxChar, cyChar);
        SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        ShowCaret(hwnd);
        return 0;

    case WM_KILLFOCUS:
        HideCaret(hwnd);
        DestroyCaret();
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_HOME:
            xCaret = 0;
            break;

        case VK_END:
            xCaret = cxBuffer - 1;
            break;

        case VK_PRIOR:
            yCaret = 0;
            break;

        case VK_NEXT:
            yCaret = cyBuffer - 1;
            break;

        case VK_LEFT:
            xCaret = max(xCaret - 1, 0);
            break;

        case VK_RIGHT:
            xCaret = min(xCaret + 1, cxBuffer - 1);
            break;

        case VK_UP:
            yCaret = max(yCaret - 1, 0);
            break;

        case VK_DOWN:
            yCaret = min(yCaret + 1, cyBuffer - 1);
            break;

        case VK_DELETE:
            for (x = xCaret; x < cxBuffer - 1; x++)
                BUFFER(x, yCaret) = BUFFER(x + 1, yCaret);
            BUFFER(cxBuffer - 1, yCaret) = ' ';

            HideCaret(hwnd);

            hdc = GetDC(hwnd);

            SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas")));

            TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), cxBuffer - xCaret);

            DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
            ReleaseDC(hwnd, hdc);
            ShowCaret(hwnd);
            break;
        }
        SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        return 0;

    case WM_CHAR:
        for (i = 0; i < (int)LOWORD(lParam); i++)
        {
            switch (wParam)
            {
            case '\b': // backspace
                if (xCaret > 0)
                {
                    xCaret--;
                    SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 1);
                }
                break;

            case '\t': // tab
                do
                {
                    SendMessage(hwnd, WM_CHAR, ' ', 1);
                } while (xCaret % 4 != 0);
                break;

            case '\n': // line feed
                if (++yCaret == cyBuffer)
                    yCaret = 0;
                break;

            case '\r': // carriage return
                xCaret = 0;
                if (++yCaret == cyBuffer)
                    yCaret = 0;
                break;

            case '\x1B': // escape
                for (y = 0; y < cyBuffer; y++)
                    for (x = 0; x < cxBuffer; x++)
                        BUFFER(x, y) = ' ';

                xCaret = 0;
                yCaret = 0;

                InvalidateRect(hwnd, NULL, FALSE);
                break;

            default: // character codes
                BUFFER(xCaret, yCaret) = (TCHAR)wParam;

                HideCaret(hwnd);
                hdc = GetDC(hwnd);
                SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas")));

                TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), 1);

                DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
                ReleaseDC(hwnd, hdc);
                ShowCaret(hwnd);

                if (++xCaret == cxBuffer)
                {
                    xCaret = 0;
                    if (++yCaret == cyBuffer)
                        yCaret = 0;
                }
                break;
            }
        }

        SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas")));

        for (y = 0; y < cyBuffer; y++)
            TextOut(hdc, 0, y * cyChar, &BUFFER(0, y), cxBuffer);

        DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
