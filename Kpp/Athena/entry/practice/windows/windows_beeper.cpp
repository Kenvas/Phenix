#include "PrecompiledHeader.hpp"

#include "begin"

class BeeperWindow : public gui::Window
{
private:

    bool bFlipFlop = false;

protected:

    virtual bool OnCreate() override
    {
        SetTimer(GetWindowHandle(), 1, 500, nullptr);
        return true;
    }

    virtual void OnDestroy() override
    {
        KillTimer(GetWindowHandle(), 1);
    }

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd   = GetWindowHandle();
        autox hdc    = HDC();
        autox ps     = PAINTSTRUCT();
        autox rect   = RECT();
        autox hbrush = HBRUSH();

        switch (message)
        {
        case WM_TIMER:
            MessageBeep(-1);
            bFlipFlop = !bFlipFlop;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            hbrush = CreateSolidBrush(bFlipFlop ? RGB(255, 0, 0) : RGB(0, 0, 255));
            FillRect(hdc, &rect, hbrush);
            EndPaint(hwnd, &ps);
            DeleteObject(hbrush);
            return 0;
        }

        return Window::OnEvent(message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<BeeperWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"