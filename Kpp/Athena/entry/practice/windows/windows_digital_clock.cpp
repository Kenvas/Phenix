#include "PrecompiledHeader.hpp"

#include "begin"

class DigitalClockWindow : public gui::Window
{
private:

    bool f24Hour_;
    bool fSuppress_;
    HBRUSH hBrushRed_;

    bool fSevenSegment[10][7] =
    {
        1, 1, 1, 0, 1, 1, 1, // 0
        0, 0, 1, 0, 0, 1, 0, // 1
        1, 0, 1, 1, 1, 0, 1, // 2
        1, 0, 1, 1, 0, 1, 1, // 3
        0, 1, 1, 1, 0, 1, 0, // 4
        1, 1, 0, 1, 0, 1, 1, // 5
        1, 1, 0, 1, 1, 1, 1, // 6
        1, 0, 1, 0, 0, 1, 0, // 7
        1, 1, 1, 1, 1, 1, 1, // 8
        1, 1, 1, 1, 0, 1, 1, // 9
    };

    POINT ptSegment [7][6] =
    {
        7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
        6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
        36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
        7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
        6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
        36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
        7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70
    };

    POINT ptColon [2][4] =
    {
        2,  21,  6,  17,  10, 21,  6, 25,
        2,  51,  6,  47,  10, 51,  6, 55
    };

    void DisplayDigit(HDC hdc, int iNumber)
    {
        for (int iSeg = 0; iSeg < 7; iSeg++)
        {
            if (fSevenSegment[iNumber][iSeg])
                Polygon(hdc, ptSegment[iSeg], 6);
        }
    }

    void DisplayTwoDigits(HDC hdc, int iNumber, bool fSuppress)
    {
        if (!fSuppress || (iNumber / 10 != 0))
            DisplayDigit(hdc, iNumber / 10);
        OffsetWindowOrgEx(hdc, -42, 0, nullptr);
        DisplayDigit(hdc, iNumber % 10);
        OffsetWindowOrgEx(hdc, -42, 0, nullptr);
    }

    void DisplayColon(HDC hdc)
    {
        Polygon(hdc, ptColon[0], 4);
        Polygon(hdc, ptColon[1], 4);
        OffsetWindowOrgEx(hdc, -12, 0, nullptr);
    }

    void DisplayTime(HDC hdc, bool f24Hour, bool fSuppress)
    {
        autox st = SYSTEMTIME();

        GetLocalTime(&st);

        if (f24Hour)
            DisplayTwoDigits(hdc, st.wHour, fSuppress);
        else
            DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);

        DisplayColon(hdc);
        DisplayTwoDigits(hdc, st.wMinute, FALSE);
        DisplayColon(hdc);
        DisplayTwoDigits(hdc, st.wSecond, FALSE);
    }

    void OnSettingChanged()
    {
        TCHAR szBuffer[2];

        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
        f24Hour_ = (szBuffer[0] == '1');

        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
        fSuppress_ = (szBuffer[0] == '0');

        InvalidateRect(GetWindowHandle(), nullptr, TRUE);
    }

protected:

    virtual bool OnCreate() override
    {
        hBrushRed_ = CreateSolidBrush(RGB(255, 0, 0));
        SetTimer(GetWindowHandle(), 1, 1000, nullptr);
        return true;
    }

    virtual void OnDestroy() override
    {
        KillTimer(GetWindowHandle(), 1);
        DeleteObject(hBrushRed_);
    }

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override
    {
        autox hwnd = GetWindowHandle();
        autox hdc  = HDC();
        autox ps   = PAINTSTRUCT();
        autox size = GetSize();

        switch (message)
        {
        case WM_TIMER:
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            SetMapMode(hdc, MM_ISOTROPIC);
            SetWindowExtEx(hdc, 276, 72, nullptr);
            SetViewportExtEx(hdc, size.width, size.height, nullptr);

            SetWindowOrgEx(hdc, 138 + 15, 36, nullptr);
            SetViewportOrgEx(hdc, size.width / 2, size.height / 2, nullptr);
            SelectObject(hdc, GetStockObject(NULL_PEN));
            SelectObject(hdc, hBrushRed_);

            DisplayTime(hdc, f24Hour_, fSuppress_);

            EndPaint(hwnd, &ps);
            return 0;
        }

        return Window::OnEvent(message, wparam, lparam);
    }

};

KV_QuickAddEntry
{
    autox app = make_shared<DigitalClockWindow>();

    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};

#include "end"