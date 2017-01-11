#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/native/windows/utils.hpp"


#include "fmt/format.h"
#include "fmt/time.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define FNX_PRINT_MESSAGE(name, value) std::cout << "message: " \
    << termcolor::cyan << name \
    << termcolor::green << fmt::format("(0x{0:04x})", value) \
    << termcolor::reset << std::endl

LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
bool RegisterWindowClass(PCTSTR const class_name, WNDPROC window_procdure);
void UnregisterWindowClass(PCTSTR const class_name);
HWND CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_name);

KV_QuickAddEntry
{
    PCTSTR const class_name = TEXT("PhenixAppExample");

    RegisterWindowClass(class_name, WindowProcedure);

    autox WindowHandle_ = CreateWindowInstance(class_name, TEXT("example"));

    ShowWindow(WindowHandle_, SW_SHOWDEFAULT);
    UpdateWindow(WindowHandle_);

    autox evtarg = MSG();

    while (GetMessage(&evtarg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&evtarg);
        DispatchMessage(&evtarg);
    }

    DestroyWindow(WindowHandle_);
    UnregisterWindowClass(class_name);

    return 0;
};

LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    autox duration = high_resolution_clock::now().time_since_epoch();
    autox micros   = (duration_cast<std::chrono::microseconds>(duration).count() % 1000000);
    autox t        = std::time(nullptr);
    autox tm       = std::localtime(&t);
    std::cout << fmt::format("{0:%T}.{1:06d} ", *tm, micros)
    //std::cout << "message: "
        << termcolor::cyan      << fmt::format("{0:^20}", kv::native::windows::utils::GetWindowMessageName(message))
        << termcolor::green     << fmt::format("(0x{0:04x}) ", message)
        << termcolor::magenta   << fmt::format("wp: 0x{0:016x} lp: 0x{1:016x} ", wparam, lparam)
        << termcolor::reset     << std::endl;
    switch (message)
    {
    case WM_DESTROY:
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
    UnregisterClass(class_name, GetModuleHandle(nullptr));
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