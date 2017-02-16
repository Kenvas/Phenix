#include "PrecompiledHeader.hpp"

#include "begin"

#define BUFFER(x, y) *(pBuffer + y * cxBuffer + x)

class TyperWindow : public gui::Window
{
private:

    DWORD dwCharSet = DEFAULT_CHARSET;
    int cxChar, cyChar;
    int cxBuffer, cyBuffer;
    int xCaret, yCaret;
    TCHAR *pBuffer = nullptr;
    HFONT hfont;

protected:

    virtual bool OnCreate() override
    {
        autox hwnd = GetWindowHandle();
        autox hdc  = HDC();
        autox tm   = TEXTMETRIC();

        hdc = GetDC(hwnd);
        hfont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas"));
        SelectObject(hdc, hfont);
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight;

        DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
        ReleaseDC(hwnd, hdc);

        OnSize(0, 0);

        return true;
    }

    virtual void OnDestroy() override
    {
        if (pBuffer != nullptr) free(pBuffer);
    }

    virtual void OnSize(int width, int height) override
    {
        autox hwnd = GetWindowHandle();

        cxBuffer = max(1, width / cxChar);
        cyBuffer = max(1, height/ cyChar);

        if (pBuffer != nullptr) free(pBuffer);

        pBuffer = (TCHAR *)malloc(sizeof(TCHAR) * cxBuffer * cyBuffer);

        for (int y = 0; y < cyBuffer; y++)
            for (int x = 0; x < cxBuffer; x++)
                BUFFER(x, y) = ' ';

        xCaret = 0;
        yCaret = 0;

        if (hwnd == GetFocus())
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);

        InvalidateRect(hwnd, nullptr, TRUE);
    }

    virtual void OnFocus(bool enable) override
    {
        autox hwnd = GetWindowHandle();

        if (enable)
        {
            CreateCaret(hwnd, nullptr, 2, cyChar);
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
            ShowCaret(hwnd);
        }
        else
        {
            HideCaret(hwnd);
            DestroyCaret();
        }
    }

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();
        autox hdc  = HDC();
        autox ps   = PAINTSTRUCT();

        switch (message)
        {
        case WM_INPUTLANGCHANGE:
            dwCharSet = (DWORD)wparam;
            OnCreate();
            return 0;

        case WM_KEYDOWN:
            switch (wparam)
            {
            case VK_HOME : xCaret = 0;                             break;
            case VK_END  : xCaret = cxBuffer - 1;                  break;
            case VK_PRIOR: yCaret = 0;                             break;
            case VK_NEXT : yCaret = cyBuffer - 1;                  break;
            case VK_LEFT : xCaret = max(xCaret - 1, 0);            break;
            case VK_RIGHT: xCaret = min(xCaret + 1, cxBuffer - 1); break;
            case VK_UP   : yCaret = max(yCaret - 1, 0);            break;
            case VK_DOWN : yCaret = min(yCaret + 1, cyBuffer - 1); break;

            case VK_DELETE:
                for (int x = xCaret; x < cxBuffer - 1; x++)
                    BUFFER(x, yCaret) = BUFFER(x + 1, yCaret);
                BUFFER(cxBuffer - 1, yCaret) = ' ';

                HideCaret(hwnd);

                hdc = GetDC(hwnd);
                hfont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
                    dwCharSet, 0, 0, 0, FIXED_PITCH, TEXT("Consolas"));
                SelectObject(hdc, hfont);

                TextOut(hdc, xCaret * cxChar, yCaret * cyChar, &BUFFER(xCaret, yCaret), cxBuffer - xCaret);

                DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
                ReleaseDC(hwnd, hdc);
                ShowCaret(hwnd);
                break;
            }
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
            return 0;

        case WM_CHAR:
            for (int i = 0; i < (int)LOWORD(lparam); i++)
            {
                switch (wparam)
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
                    for (int y = 0; y < cyBuffer; y++)
                        for (int x = 0; x < cxBuffer; x++)
                            BUFFER(x, y) = ' ';

                    xCaret = 0;
                    yCaret = 0;

                    InvalidateRect(hwnd, nullptr, FALSE);
                    break;

                default: // character codes
                    BUFFER(xCaret, yCaret) = (TCHAR)wparam;

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

            for (int y = 0; y < cyBuffer; y++)
                TextOut(hdc, 0, y * cyChar, &BUFFER(0, y), cxBuffer);

            DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));

            EndPaint(hwnd, &ps);
            return 0;
        }

        return Window::OnEvent(message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<TyperWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"