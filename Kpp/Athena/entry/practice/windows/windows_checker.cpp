#include "PrecompiledHeader.hpp"

#include "begin"


#define CHECKERWINDOW_VERSION 2
#define kvmax(a, b) ((a) > (b) ? (a) : (b))
#define kvmin(a, b) ((a) < (b) ? (a) : (b))

static constexpr int RowCount() noexcept { return 5; }
static constexpr int ColumnCount() noexcept { return 5; }

class CheckerWindow : public gui::Window
{
private:

    BOOL fState[RowCount()][ColumnCount()];
    int cxBlock, cyBlock;
    int mx, my;

protected:

    virtual void OnSize(int width, int height) override
    {
        cxBlock = width / ColumnCount();
        cyBlock = height / RowCount();
    }

#if CHECKERWINDOW_VERSION >= 2
    virtual void OnFocus(bool enable) override
    {
        ShowCursor(enable ? TRUE : FALSE);
    }
#endif

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();

        autox rect = RECT();
        autox hdc  = HDC();
        autox ps   = PAINTSTRUCT();
        autox pt   = POINT();

        switch (message)
        {
#if CHECKERWINDOW_VERSION >= 2
        case WM_KEYDOWN:
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);

            mx = kvmax(0, kvmin(ColumnCount() - 1, pt.x / cxBlock));
            my = kvmax(0, kvmin(RowCount() - 1, pt.y / cyBlock));

            switch (wparam)
            {
            case VK_UP    : my--;                                        break;
            case VK_DOWN  : my++;                                        break;
            case VK_LEFT  : mx--;                                        break;
            case VK_RIGHT : mx++;                                        break;
            case VK_HOME  : mx = my = 0;                                 break;
            case VK_END   : mx = ColumnCount() - 1; my = RowCount() - 1; break;
            case VK_SPACE :
            case VK_RETURN: SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON,
                MAKELONG(mx * cxBlock, my * cyBlock));                   break;
            default       :                                              break;
            }

            mx = (mx + ColumnCount()) % ColumnCount();
            my = (my + RowCount()) % RowCount();

            pt.x = mx * cxBlock + cxBlock / 2;
            pt.y = my * cyBlock + cyBlock / 2;

            ClientToScreen(hwnd, &pt);
            SetCursorPos(pt.x, pt.y);

            return 0;
#endif

        case WM_LBUTTONDOWN:
        {
            mx = (LOWORD(lparam) / cxBlock);
            my = (HIWORD(lparam) / cyBlock);
            autox bx = (0 <= mx && mx < ColumnCount());
            autox by = (0 <= my && my < RowCount());
            if (bx && by)
            {
                fState[my][mx] ^= 1;

                rect.left   = mx       * cxBlock;
                rect.top    = my       * cyBlock;
                rect.right  = (mx + 1) * cxBlock;
                rect.bottom = (my + 1) * cyBlock;

                InvalidateRect(hwnd, &rect, FALSE);
            }
            else
            {
                MessageBeep(0);
            }
            return 0;
        }

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            for (int y = 0; y < RowCount(); y++)
            {
                for (int x = 0; x < ColumnCount(); x++)
                {
                    auto l = (x       * cxBlock);
                    auto t = (y       * cyBlock);
                    auto r = ((x + 1) * cxBlock);
                    auto b = ((y + 1) * cyBlock);
                    Rectangle(hdc, l, t, r, b);
                    if (fState[y][x])
                    {
                        MoveToEx(hdc, l, t, nullptr);
                        LineTo(hdc, r, b);
                        MoveToEx(hdc, r, t, nullptr);
                        LineTo(hdc, l, b);
                    }
                }
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        return Window::OnEvent(message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<CheckerWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"