#include "PrecompiledHeader.hpp"

#include "kv/native/windows/wgl.hpp"
#include "kv/log/IncludeAll.hpp"

#include "termcolor/termcolor.hpp"
#include "fmt/format.h"

using namespace std;

namespace kv { namespace native { namespace windows { namespace wgl
{
    static PFNWGLGETEXTENSIONSSTRINGARBPROC  __wglGetExtensionsStringARB  = nullptr;
    static PFNWGLCREATECONTEXTATTRIBSARBPROC __wglCreateContextAttribsARB = nullptr;
    static PFNWGLCHOOSEPIXELFORMATARBPROC    __wglChoosePixelFormatARB    = nullptr;
    static PFNWGLSWAPINTERVALEXTPROC         __wglSwapIntervalEXT         = nullptr;
    static PFNWGLGETSWAPINTERVALEXTPROC      __wglGetSwapIntervalEXT      = nullptr;

    BOOL WINAPI _LoadExtensions()
    {
        autox retval = TRUE;
        cout << termcolor::green << "info: load wgl extensions" << endl;
    #define FNX_GET_PROC_ADDR(func_proto, ext_name) __##ext_name = (func_proto)wglGetProcAddress(#ext_name); \
        do { \
            if (__##ext_name == nullptr) { \
                cout << termcolor::magenta << fmt::format("    {0:<26}", #ext_name) \
                    << termcolor::red << " failed." \
                    << termcolor::reset << endl; \
                retval = FALSE; \
            } else { \
                cout << termcolor::magenta << fmt::format("    {0:<26}", #ext_name) \
                    << termcolor::green << " success." \
                    << termcolor::reset << endl; \
            } \
        } while (0)
        FNX_GET_PROC_ADDR(PFNWGLGETEXTENSIONSSTRINGARBPROC , wglGetExtensionsStringARB );
        FNX_GET_PROC_ADDR(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
        FNX_GET_PROC_ADDR(PFNWGLCHOOSEPIXELFORMATARBPROC   , wglChoosePixelFormatARB   );
        FNX_GET_PROC_ADDR(PFNWGLSWAPINTERVALEXTPROC        , wglSwapIntervalEXT        );
        FNX_GET_PROC_ADDR(PFNWGLGETSWAPINTERVALEXTPROC     , wglGetSwapIntervalEXT     );
    #undef FNX_GET_PROC_ADDR
        return retval;
    }

    static LRESULT WINAPI OnEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
    {
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    BOOL WINAPI LoadExtensions()
    {
        static autox init = FALSE;

        if (init) return TRUE;

        autox dispsose = [](int step, LPCTSTR class_name, HINSTANCE instance, HWND hwnd, HDC hdc, HGLRC hglrc)
        {
            switch (step)
            {
            case 5: wgl::MakeCurrent(nullptr, nullptr);
            case 4: wgl::DeleteContext(hglrc);
            case 3: ReleaseDC(hwnd, hdc);
            case 2: DestroyWindow(hwnd);
            case 1: UnregisterClass(class_name, instance);
            default: break;
            }
        };

        TCHAR class_name[] = TEXT("PhenixTemporaryWindowWGL");
        autox instance     = GetModuleHandle(nullptr);
        autox wcex         = WNDCLASSEX{ 0 };
        wcex.cbSize        = sizeof(wcex);
        wcex.hInstance     = instance;
        wcex.lpfnWndProc   = &OnEvent;
        wcex.lpszClassName = class_name;

        if (!RegisterClassEx(&wcex))
        {
            log::simple->error("error: register window class failed.");
            dispsose(0, nullptr, nullptr, nullptr, nullptr, nullptr);
            return FALSE;
        }

        autox hwnd = CreateWindowEx(WS_EX_APPWINDOW, class_name, nullptr, WS_OVERLAPPEDWINDOW,
            0, 0, 0, 0, nullptr, nullptr, instance, nullptr);
        if (hwnd == nullptr)
        {
            log::simple->error("error: create window instance failed");
            dispsose(1, class_name, instance, nullptr, nullptr, nullptr);
            return FALSE;
        }

        autox hdc = GetDC(hwnd);
        if (hdc == nullptr)
        {
            log::simple->error("error: get device handle failed.");
            dispsose(2, class_name, instance, hwnd, nullptr, nullptr);
            return FALSE;
        }

        autox pfd = PIXELFORMATDESCRIPTOR{ 0 };
        if (!SetPixelFormat(hdc, 1, &pfd))
        {
            log::simple->error("error: set pixel format failed.");
            dispsose(3, class_name, instance, hwnd, hdc, nullptr);
            return FALSE;
        }

        autox hglrc = wgl::CreateContext(hdc);
        if (hglrc == nullptr)
        {
            log::simple->error("error: create rendering context failed.");
            dispsose(3, class_name, instance, hwnd, hdc, nullptr);
            return FALSE;
        }

        if (!wgl::MakeCurrent(hdc, hglrc))
        {
            log::simple->error("error: make current context failed.");
            dispsose(4, class_name, instance, hwnd, hdc, hglrc);
            return FALSE;
        }

        _LoadExtensions();
        dispsose(5, class_name, instance, hwnd, hdc, hglrc);

        init = TRUE;

        return init;
    }

    BOOL WINAPI IsExtensionSupported(HDC hdc, LPCSTR extName)
    {
        auto extensions = GetExtensionsString(hdc);
        auto position   = strstr(extensions, extName);
        auto supported  = (position != nullptr);
        return supported;
    }

    LPCSTR WINAPI GetExtensionsString(HDC hdc)
    {
        return (__wglGetExtensionsStringARB == nullptr) ? nullptr
            : __wglGetExtensionsStringARB(hdc);
    }

    HGLRC WINAPI CreateContextAttribs(HDC hdc, HGLRC hShareContext, const int * attribList)
    {
        return (__wglCreateContextAttribsARB == nullptr) ? nullptr
            : __wglCreateContextAttribsARB(hdc, hShareContext, attribList);
    }

    BOOL WINAPI ChoosePixelFormat(HDC hdc, const int *piAttribIList, const float *pfAttribFList, int nMaxFormats, int *piFormats, int *nNumFormats)
    {
        return (__wglChoosePixelFormatARB == nullptr) ? false
            : __wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, (UINT *)nNumFormats);
    }

    BOOL WINAPI SwapInterval(int interval)
    {
        return (__wglSwapIntervalEXT == nullptr) ? false
            : __wglSwapIntervalEXT(interval);
    }

    int WINAPI GetSwapInterval(void)
    {
        return (__wglGetSwapIntervalEXT == nullptr) ? 0
            : __wglGetSwapIntervalEXT();
    }

    HGLRC WINAPI CreateContext(HDC hdc)
    {
        return wglCreateContext(hdc);
    }

    BOOL WINAPI DeleteContext(HGLRC hglrc)
    {
        return wglDeleteContext(hglrc);
    }

    BOOL WINAPI MakeCurrent(HDC hdc, HGLRC hglrc)
    {
        return wglMakeCurrent(hdc, hglrc);
    }

}
}
}
}