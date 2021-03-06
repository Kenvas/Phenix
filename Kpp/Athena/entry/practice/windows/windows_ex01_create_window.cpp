#include "PrecompiledHeader.hpp"

#include "begin"

LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
bool RegisterWindowClass(PCTSTR const class_name, WNDPROC window_procdure);
void UnregisterWindowClass(PCTSTR const class_name);
HWND CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_name);

KV_QuickAddEntry
{
    PCTSTR const class_name = TEXT("__kvWindowExample__");

    RegisterWindowClass(class_name, WindowProcedure);

    autox WindowHandle_ = CreateWindowInstance(class_name, TEXT("window example"));

    ShowWindow(WindowHandle_, SW_SHOWDEFAULT);
    UpdateWindow(WindowHandle_);

    autox evtarg = MSG();

    while (GetMessage(&evtarg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&evtarg);
        DispatchMessage(&evtarg);
    }

    UnregisterWindowClass(class_name);

    return 0;
};

LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    // utils::PrintWindowMessageInfo(window_handle, message, wparam, lparam);
    switch (message)
    {
    case WM_NCDESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(window_handle, message, wparam, lparam);
}

bool RegisterWindowClass(PCTSTR const class_name, WNDPROC window_procdure)
{
    autox wcex         = WNDCLASSEX();
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;
    wcex.lpfnWndProc   = window_procdure;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = GetModuleHandle(nullptr);
    wcex.hIcon         = nullptr;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = class_name;
    wcex.hIconSm       = nullptr;
    return (RegisterClassEx(&wcex) != NULL);
}

void UnregisterWindowClass(PCTSTR const class_name)
{
    autox handle = GetModuleHandle(nullptr);
    UnregisterClass(class_name, handle);
}

HWND CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_name)
{
    autox retval = CreateWindowEx
    (
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        class_name,
        window_name,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        // 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, // x, y, width, height
        0, 0, 800, 600, // x, y, width, height
        nullptr, // parent
        nullptr, // menu
        GetModuleHandle(nullptr),
        nullptr
    );

    return retval;
}

#include "end"