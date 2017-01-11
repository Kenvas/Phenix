#include "PrecompiledHeader.hpp"

#include "kv/predef/Keyword.hpp"
#include "kv/gui/Window.hpp"
#include "kv/gui/detail/utils.hpp"
#include "kv/log/IncludeAll.hpp"

#include "termcolor/termcolor.hpp"
#include "fmt/time.h"

#include <ctime>
#include <chrono>
#include <iostream>
#include <unordered_map>

#include <windowsx.h>
#include <gl/GL.h>

#pragma comment(lib, "opengl32.lib")
// importlib("opengl32.lib")


using namespace std;
using namespace std::chrono;

//#include "asmjit/asmjit.h"
//using namespace asmjit;

namespace kv
{
    namespace gui
    {
        namespace
        {
            align_push(1)
            struct alignas(1) Thunk
            {
                struct mov_reg_imm
                {
                    uint16_t opcode;
                    size_t   value;
                };
                mov_reg_imm mov_rcx_imm;
                mov_reg_imm mov_rax_imm;
                uint16_t    jmp_rax;

                void Build(size_t this_ptr, size_t func_ptr)
                {
                    mov_rcx_imm = { 0xB948, this_ptr };
                    mov_rax_imm = { 0xB848, func_ptr };
                    jmp_rax     = { 0xE0FF };
                    FlushInstructionCache(GetCurrentProcess(), this, sizeof(*this));
                }
            };
            align_pop()
        }


        LRESULT CALLBACK Window::AuxWindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            if (message == WM_NCCREATE)
            {
                autox pcs   = reinterpret_cast<LPCREATESTRUCT>(lparam);
                autox app   = static_cast<Window * const>(pcs->lpCreateParams);
                autox thunk = static_cast<Thunk * const>(VirtualAlloc(nullptr, sizeof(Thunk), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
                autox proc  = reinterpret_cast<WNDPROC>(thunk);
                thunk->Build(reinterpret_cast<size_t>(app), reinterpret_cast<size_t>(&Window::ForwardWindowProcedure));
                if (app->WindowHandle_ == nullptr)
                    app->WindowHandle_ = window_handle;
                SetWindowLongPtr(window_handle, -4/*GWL_WNDPROC*/, (LONG_PTR)proc);
                return proc(window_handle, message, wparam, lparam);
            }
            return DefWindowProc(window_handle, message, wparam, lparam);
        }

        LRESULT CALLBACK Window::ForwardWindowProcedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            autox app = reinterpret_cast<Window * const>(window_handle);
            {
                autox dt = high_resolution_clock::now().time_since_epoch();
                autox us = (duration_cast<microseconds>(dt).count() % microseconds::period::den);
                autox t  = std::time(nullptr);
                autox tm = std::localtime(&t);
                std::cout << fmt::format("{0:%T}.{1:06d} ", *tm, us)
                    << termcolor::cyan    << fmt::format("{0:^20}", utils::GetWindowMessageName(message))
                    << termcolor::green   << fmt::format("(0x{0:04x}) ", message)
                    << termcolor::magenta << fmt::format("wp:0x{0:016x} lp:0x{1:016x} ", wparam, lparam)
                    << termcolor::reset   << std::endl;
            }
            return app->OnEvent(message, wparam, lparam);
        }

        Window::Window() noexcept
            : Window(CW_USEDEFAULT, CW_USEDEFAULT)
        { }

        Window::Window(int width, int height) noexcept
            : Size_          (width, height)
            , WindowHandle_  (nullptr)
            , StartupTime_   (ClockType::now())
        { }

        Window::~Window() noexcept
        {}

        Size const & Window::GetSize() const noexcept
        {
            return Size_;
        }

        void Window::SetSize(int width, int height)
        {
            Size_ = { width, height };
        }

        bool Window::Initialize()
        {
            if (WindowHandle_ != nullptr)
                return true;

            autox class_name  = GetWindowClassName();
            autox window_name = GetWindowTitle();

            if (!RegisterWindowClass(class_name, Window::AuxWindowProcedure))
            {
                GetLastError();
                log::simple->error("error: register window class failed.");
                return false;
            }
            log::simple->info(log::color::green, "info: register window class success.");

            WindowHandle_ = CreateWindowInstance(class_name, window_name);
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

        int Window::Run() const
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
                        TranslateMessage(&evtarg);
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

        void Window::OnCreate()
        {
        }

        void Window::OnDestroy()
        {
        }

        void Window::OnClose()
        {
        }

        void Window::OnResize(int width, int height)
        {
        }

        HWND const Window::GetWindowHandle() const noexcept
        {
            return WindowHandle_;
        }

        PCTSTR const Window::GetWindowClassName() const noexcept
        {
            return TEXT("PhenixWindownApp");
        }

        PCTSTR const Window::GetWindowTitle() const noexcept
        {
            return TEXT("PhenixWindow");
        }

        inline bool Window::RegisterWindowClass(PCTSTR const class_name, WNDPROC const window_procdure) const noexcept
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

        void Window::UnregisterWindowClass(PCTSTR const class_name) const noexcept
        {
            UnregisterClass(class_name, GetModuleHandle(nullptr));
        }

        HWND Window::CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_title)
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

        bool Window::InitializeDetail()
        {
            return true;
        }

        LRESULT CALLBACK Window::OnEvent(UINT message, WPARAM wparam, LPARAM lparam)
        {
            switch (message)
            {
            case WM_CREATE:
                break;
            case WM_DESTROY:
                OnDestroy();
                break;
            case WM_SIZE:
                OnResize(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
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