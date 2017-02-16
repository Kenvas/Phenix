#include "PrecompiledHeader.hpp"

#include "begin"

#define MAXPOINTS 1000

class ConnectWindow : public gui::Window
{
private:

    POINT pt[MAXPOINTS];
    int iCount;

protected:

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();

        autox hdc = HDC();
        autox ps  = PAINTSTRUCT();

        switch (message)
        {
        case WM_LBUTTONDOWN:
            iCount = 0;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;

        case WM_MOUSEMOVE:
            if ((wparam & MK_LBUTTON) && (iCount < 1000))
            {
                autox x = LOWORD(lparam);
                autox y = HIWORD(lparam);
                pt[iCount].x = x;
                pt[iCount].y = y;
                iCount++;
                hdc = GetDC(hwnd);
                SetPixel(hdc, x, y, 0);
                ReleaseDC(hwnd, hdc);
            }
            return 0;

        case WM_LBUTTONUP:
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SetCursor(LoadCursor(nullptr, IDC_WAIT));
            ShowCursor(TRUE);
            for (int i = 0; i < iCount - 1; i++)
            {
                for (int j = i + 1; j < iCount; j+=int(sqrt(iCount)))
                {
                    MoveToEx(hdc, pt[i].x, pt[i].y, nullptr);
                    LineTo(hdc, pt[j].x, pt[j].y);
                }
            }
            ShowCursor(FALSE);
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
            EndPaint(hwnd, &ps);
            return 0;
        }

        return Window::OnEvent(message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<ConnectWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"