#include "PrecompiledHeader.hpp"

#include "../Window.hpp"
#include "kv/native/windows/Utils.hpp"
#include "kv/native/windows/wgl.hpp"
#include "kv/log/IncludeAll.hpp"
#include "kv/predef/Keyword.hpp"

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
using namespace kv::native::windows;

//#include "asmjit/asmjit.h"
//using namespace asmjit;

#include "../_namespace/begin"

thread_local static void * ThreadLocalData = nullptr;


align_push(1)
struct alignas(1) kvWindowProcedureThunk
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


Window::Window() noexcept
    : Size_          (CW_USEDEFAULT, CW_USEDEFAULT)
    , WindowHandle_  (nullptr)
    , StartupTime_   (ClockType::now())
{}

Window::~Window() noexcept
{}

Size2i const & Window::GetSize() const noexcept
{
    return Size_;
}

void Window::SetSize(int width, int height) noexcept
{
    Size_.width  = width;
    Size_.height = height;
}

bool Window::Initialize()
{
    if (WindowHandle_ != nullptr)
        return true;

    autox class_name  = GetWindowClassName();

    autox procedure = [](HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
    {
        auto _procedure = static_cast<WNDPROC>(
        [](HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
        {
            auto window = reinterpret_cast<Window * const>(window_handle);
            utils::PrintWindowMessageInfo(window->GetWindowHandle(), message, wparam, lparam);
            if (message == WM_NCDESTROY)
            {
                auto pointer = GetWindowLongPtr(window->GetWindowHandle(), /*GWL_WNDPROC*/-4);
                VirtualFree(reinterpret_cast<LPVOID>(pointer), 0, MEM_RELEASE);
                utils::PrintErrorMessageInfo();
                PostQuitMessage(0);
            }
            return window->OnEvent(message, wparam, lparam);
        });
        auto _function  = VirtualAlloc(nullptr, sizeof(kvWindowProcedureThunk)
            , MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        auto _thunk     = static_cast<kvWindowProcedureThunk * const>(_function);
        auto _window    = static_cast<Window * const>(ThreadLocalData);
        utils::PrintErrorMessageInfo();
        _thunk->Build(reinterpret_cast<size_t>(_window), reinterpret_cast<size_t>(_procedure));
        SetWindowLongPtr(window_handle, /*GWL_WNDPROC*/-4, reinterpret_cast<LONG_PTR>(_function));
        _window->WindowHandle_ = window_handle;
        return _window->OnEvent(message, wparam, lparam);
    };

    if (!utils::RegisterWindowClass(class_name, procedure))
    {
        log::error(log::color::red)("error: register window class failed.")();
        return false;
    }
    log::info(log::color::green)("info: register window class success.")();

    ThreadLocalData = static_cast<void *>(this);
    WindowHandle_   = CreateWindowInstance();
    if (WindowHandle_ == nullptr)
    {
        utils::UnregisterWindowClass(class_name);
        log::error(log::color::red)("error: create window instance failed.")();
        return false;
    }
    log::info(log::color::green)("info: create window instance success.")();

    if (Size_.width == CW_USEDEFAULT || Size_.height == CW_USEDEFAULT)
    {
        autox client = RECT{};
        GetClientRect(WindowHandle_, &client);
        Size_.width  = client.right - client.left;
        Size_.height = client.bottom - client.top;
    }

    autox screen = Size2i{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    SetWindowPos(WindowHandle_, HWND_TOP
        , (screen.width - Size_.width) / 2, (screen.height - Size_.height) / 2
        , Size_.width, Size_.height, SWP_SHOWWINDOW);
    //ShowWindow(WindowHandle_, SW_SHOWDEFAULT);
    UpdateWindow(WindowHandle_);

    return true;
}

int Window::Run()
{
    autox evtarg = MSG();

    while (true)
    {
        if (PeekMessage(&evtarg, nullptr, 0, 0, PM_REMOVE))
        {
            if (evtarg.message == WM_QUIT) break;
            TranslateMessage(&evtarg);
            DispatchMessage(&evtarg);
        }

        DoUpdate();
        DoRender();
    }

    utils::UnregisterWindowClass(GetWindowClassName());

    return static_cast<int>(evtarg.wParam);
}

void Window::DoUpdate()
{
}

void Window::DoRender()
{
}

bool Window::OnCreate()
{
    return true;
}

void Window::OnDestroy()
{
}

void Window::OnClose()
{
    DefWindowProc(WindowHandle_, WM_CLOSE, 0, 0);
}

void Window::OnFocus(bool enable)
{
}

void Window::OnSize(int width, int height)
{
}

HWND Window::GetWindowHandle() const noexcept
{
    return WindowHandle_;
}

PCTSTR Window::GetWindowClassName() const noexcept
{
    return TEXT("__kvWindow_1_0_0__");
}

HWND Window::CreateWindowInstance()
{
    autox retval = CreateWindowEx
    (
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        GetWindowClassName(),
        TEXT("Window"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        // 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, // x, y, width, height
        CW_USEDEFAULT, CW_USEDEFAULT, Size_.width, Size_.height,
        nullptr, // parent
        nullptr, // menu
        GetModuleHandle(nullptr),
        this // in message WM_NCCREATE, WM_CREATE, we can get 'this' pointer from 'lparam'
    );

    return retval;
}

LRESULT CALLBACK Window::OnEvent(UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CREATE:
        return (OnCreate() ? 0 : -1);
    case WM_DESTROY:
        return (OnDestroy(), 0);
    case WM_SIZE:
        SetSize(LOWORD(lparam), HIWORD(lparam));
        return (OnSize(LOWORD(lparam), HIWORD(lparam)), 0);
    case WM_SETFOCUS:
        return (OnFocus(true), 0);
    case WM_KILLFOCUS:
        return (OnFocus(false), 0);
    case WM_CLOSE:
        return (OnClose(), 0);
    case WM_PAINT:
        break;
    case WM_NCCREATE: // non client create
        break;
    case WM_NCDESTROY:   // non client destroy
        break;
    case WM_NCHITTEST:   // non client hit test
        break;
    case WM_KEYDOWN:
        break;
    case WM_KEYUP:
        break;
    case WM_CHAR:
        break;
    case WM_TIMER:
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
    case WM_XBUTTONDOWN:
        break;
    case WM_XBUTTONUP:
        break;
    case WM_XBUTTONDBLCLK:
        break;
    case WM_MOUSEWHEEL:
        break;
    case WM_MOUSEHWHEEL:
        break;
    default:
        break;
    }
    autox retval = DefWindowProc(WindowHandle_, message, wparam, lparam);
    log::debug("(0x{0:04x}) {1}", message, retval)();
    return retval;
}

#include "../_namespace/end"