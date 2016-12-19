#include "phenix/app/WindowApp.h"
#include "phenix/app/detail/utils.h"
#include "phenix/log/Log.h"

#include "termcolor/termcolor.hpp"
#include "fmt/time.h"

#include <ctime>
#include <chrono>
#include <iostream>
#include <unordered_map>

#pragma comment(lib, "opengl32.lib")
#include <gl/GL.h>

using namespace std;
using namespace std::chrono;

//#include "asmjit/asmjit.h"
//using namespace asmjit;

//#define FNX_PRINT_MESSAGE_TEMPLATE(name, value, prefix, wparam, lparam) \
//    do { \
//        auto duration = high_resolution_clock::now().time_since_epoch(); \
//        auto micros = duration_cast<std::chrono::microseconds>(duration).count() % 1000000; \
//        auto t = std::time(nullptr); \
//        auto tm = std::tm(); \
//        ::localtime_s(&tm, &t); \
//        std::cout << fmt::format("{0:%T}.{1:06d} ", tm, micros) \
//        << termcolor::cyan << fmt::format("{0:^18}", prefix##name) \
//        << termcolor::green << fmt::format("(0x{0:04x}) ", value) \
//        << termcolor::magenta << fmt::format("wp:0x{0:016x} lp: 0x{1:016x} ", wparam, lparam) \
//        << termcolor::reset << std::endl; \
//    } while (0)
//#define FNX_PRINT_MESSAGE(name, code, wparam, lparam) FNX_PRINT_MESSAGE_TEMPLATE(name, code, "", wparam, lparam)
//#define FNX_PRINT_MESSAGE_STATIC(name, code, wparam, lparam) FNX_PRINT_MESSAGE_TEMPLATE(name, code, "$", wparam, lparam)

namespace fnx
{
    namespace app
    {
        namespace
        {
#pragma pack(push,1)
            struct Thunk
            {
                struct mov_reg_imm
                {
                    uint16_t opcode;
                    uint64_t value;
                };
                mov_reg_imm mov_rcx_imm;
                mov_reg_imm mov_rax_imm;
                uint16_t    jmp_rax;
            };
#pragma pack(pop)
        }

        LRESULT CALLBACK WindowApp::TempWindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            if (message == WM_NCCREATE)
            {
                autox pcs   = reinterpret_cast<LPCREATESTRUCT>(lparam);
                autox app   = static_cast<WindowApp * const>(pcs->lpCreateParams);
                autox thunk = static_cast<Thunk * const>(VirtualAlloc(nullptr, sizeof(Thunk), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
                autox proc  = reinterpret_cast<WNDPROC>(thunk);
                thunk->mov_rcx_imm = { 0xB948, reinterpret_cast<uint64_t>(app) };
                thunk->mov_rax_imm = { 0xB848, reinterpret_cast<uint64_t>(&WindowApp::AuxWindowProcedure) };
                thunk->jmp_rax     = { 0xE0FF };
                if (app->WindowHandle_ == nullptr)
                    app->WindowHandle_ = window_handle;
                SetWindowLongPtr(window_handle, -4/*GWL_WNDPROC*/, (LONG_PTR)proc);
                return proc(window_handle, message, wparam, lparam);
            }
            return DefWindowProc(window_handle, message, wparam, lparam);
        }

        LRESULT CALLBACK WindowApp::AuxWindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            autox app = reinterpret_cast<WindowApp * const>(window_handle);
            {
                autox duration = high_resolution_clock::now().time_since_epoch();
#pragma warning(push)
#pragma warning(disable: 4552)
                autox micros   = duration_cast<microseconds>(duration).count() % microseconds::period::den;
#pragma warning(pop)
                autox time     = std::time(nullptr);
                autox tm       = std::tm();
                localtime_s(&tm, &time);
                std::cout << fmt::format("{0:%T}.{1:06d} ", tm, micros)
                    << termcolor::cyan    << fmt::format("{0:^20}", utils::GetWindowMessageName(message))
                    << termcolor::green   << fmt::format("(0x{0:04x}) ", message)
                    << termcolor::magenta << fmt::format("wp:0x{0:016x} lp: 0x{1:016x} ", wparam, lparam)
                    << termcolor::reset   << std::endl;
            }
            return app->WindowProcedure(message, wparam, lparam);
        }

        WindowApp::WindowApp() noexcept
            : WindowApp(CW_USEDEFAULT, CW_USEDEFAULT)
        { }

        WindowApp::WindowApp(int width, int height) noexcept
            : Size_          (width, height)
            , WindowHandle_  (nullptr)
            , StartupTime_   (ClockType::now())
        { }

        Size const & WindowApp::GetSize() const noexcept
        {
            return Size_;
        }

        void WindowApp::SetSize(int width, int height)
        {
            Size_ = { width, height };
        }

        bool WindowApp::Initialize()
        {
            if (WindowHandle_ != nullptr)
                return true;

            autox class_name  = GetWindowClassName();
            autox window_name = GetWindowTitle();

            if (!RegisterWindowClass(class_name, WindowApp::TempWindowProcedure))
            {
                GetLastError();
                log::simple->error("error: register window class failed.");
                return false;
            }
            log::simple->info(log::color::green, "info: register window class success.");

            WindowHandle_   = CreateWindowInstance(class_name, window_name);
            if (WindowHandle_ == nullptr)
            {
                UnregisterWindowClass(class_name);
                log::simple->error("error: create window instance failed.");
                return false;
            }
            log::simple->info(log::color::green, "info: create window instance success.");

            if (!InitializeDetail())
            {
                UnregisterWindowClass(class_name);
                return false;
            }

            ShowWindow(WindowHandle_, SW_SHOWDEFAULT);
            UpdateWindow(WindowHandle_);

            return true;
        }

        int WindowApp::Run() const
        {
            autox evtarg  = MSG();
            autox running = true;

            while (running)
            {
                if (PeekMessage(&evtarg, nullptr, 0, 0, PM_REMOVE))
                {
                    running = (evtarg.message != WM_QUIT);
                    if (running)
                    {
                        // TranslateMessage(&evtarg);
                        DispatchMessage(&evtarg);
                    }
                }
                else
                {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    SwapBuffers(GetDC(WindowHandle_));
                }
            }

            return static_cast<int>(evtarg.wParam);
        }

        void WindowApp::OnCreate()
        {
        }

        void WindowApp::OnDestroy()
        {
        }

        void WindowApp::OnClose()
        {
        }

        void WindowApp::OnResize(int width, int height)
        {
        }

        HWND const WindowApp::GetWindowHandle() const noexcept
        {
            return WindowHandle_;
        }

        PCTSTR const WindowApp::GetWindowClassName() const noexcept
        {
            return TEXT("PhenixWindownApp");
        }

        PCTSTR const WindowApp::GetWindowTitle() const noexcept
        {
            return TEXT("PhenixWindowApp");
        }

        inline bool WindowApp::RegisterWindowClass(PCTSTR const class_name, WNDPROC window_procdure) const noexcept
        {
            autox wcex         = WNDCLASSEX();
            wcex.cbSize        = sizeof(WNDCLASSEX);
            wcex.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
            wcex.lpfnWndProc   = window_procdure;
            wcex.cbClsExtra    = 0;
            wcex.cbWndExtra    = 0;
            wcex.hInstance     = GetModuleHandle(nullptr);
            wcex.hIcon         = nullptr;
            wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = nullptr; // (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName  = nullptr;
            wcex.lpszClassName = class_name;
            wcex.hIconSm       = nullptr;
            return (RegisterClassEx(&wcex) != NULL);
        }

        void WindowApp::UnregisterWindowClass(PCTSTR const class_name) const noexcept
        {
            UnregisterClass(class_name, GetModuleHandle(nullptr));
        }

        HWND WindowApp::CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_title)
        {
            autox retval = CreateWindowEx
            (
                WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                class_name,
                window_title,
                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                // 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, // x, y, width, height
                0, 0, Size_.Width, Size_.Height,
                nullptr, // parent
                nullptr, // menu
                GetModuleHandle(nullptr),
                this // in message WM_NCCREATE, WM_CREATE, we can get 'this' pointer from 'lparam'
            );

            return retval;
        }

        bool WindowApp::InitializeDetail()
        {
            return true;
        }

        LRESULT CALLBACK WindowApp::WindowProcedure(UINT message, WPARAM wparam, LPARAM lparam)
        {
            switch (message)
            {
            case WM_CREATE:
                break;
            case WM_DESTROY:
                OnDestroy();
                break;
            case WM_SIZE:
                OnResize(LOWORD(lparam), HIWORD(lparam));
                break;
            case WM_CLOSE:
                OnClose();
                break;
            case WM_PAINT:
                break;
            case WM_SETCURSOR:
                break;
            case WM_GETMINMAXINFO:
                break;
            case WM_NCCREATE:
                break;
            case WM_NCDESTROY:   // non client destroy
                {
                autox thunk = reinterpret_cast<LPVOID>(GetWindowLongPtr(WindowHandle_, -4/*GWL_WNDPROC*/));
                VirtualFree(thunk, 0, MEM_RELEASE);
                //DestroyWindow(WindowHandle_);
                UnregisterWindowClass(GetWindowClassName());
                PostQuitMessage(0);
                }
                break;
            case WM_NCCALCSIZE:
                break;
            case WM_NCHITTEST:   // non client hit test
                break;
            case WM_NCMOUSEMOVE: // non client mouse mouse
                break;
            case WM_KEYUP:
                break;
            case WM_KEYDOWN:
                break;
            case WM_CHAR:
                break;
            case WM_MOUSEMOVE:
                break;
            case WM_LBUTTONDOWN:
                break;
            case WM_LBUTTONUP:
                break;
            case WM_LBUTTONDBLCLK:
                break;
            case WM_RBUTTONDOWN:
                break;
            case WM_RBUTTONUP:
                break;
            case WM_RBUTTONDBLCLK:
                break;
            case WM_MBUTTONDOWN:
                break;
            case WM_MBUTTONUP:
                break;
            case WM_MBUTTONDBLCLK:
                break;
            case WM_MOUSEWHEEL:
                break;
            default:
                break;
            }
            autox retval = DefWindowProc(WindowHandle_, message, wparam, lparam);
            cout << fmt::format("(0x{0:04x}) {1}", message, retval) << endl;
            return retval;
        }

    }
}

//#undef FNX_PRINT_MESSAGE_TEMPLATE
//#undef FNX_PRINT_MESSAGE
//#undef FNX_PRINT_MESSAGE_STATIC