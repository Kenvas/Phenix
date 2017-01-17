#include "PrecompiledHeader.hpp"

#include "../wgl.hpp"
#include "../utils.hpp"
#include "kv/log/IncludeAll.hpp"
#include "kv/predef/Keyword.hpp"


using namespace std;
using namespace kv::native::windows;

#include "../_namespace/begin"
namespace wgl
{

static void PrintWglFunctionLoadInfo(void * value, LPCSTR const extName)
{
    if (value == nullptr)
    {
        log::debug(log::color::magenta)("    {0:<26}", extName)(log::color::red)(" failed.")();
        utils::PrintErrorInfo();
    }
    else
    {
        log::debug(log::color::magenta)("    {0:<26}", extName)(log::color::green)(" success.")();
    }
}

template <class _FunctionType>
struct FunctionProxy;

template <class _ReturnType, class ... _ArgsType>
struct FunctionProxy<_ReturnType(WINAPI *)(_ArgsType ...)>
{
private:
    using function_t = _ReturnType(WINAPI *)(_ArgsType ...);
    function_t value;
public:
    FunctionProxy() : value(function_t(-1)) {}
    inline bool load(LPCSTR const extName) noexcept
    {
        if (value == function_t(-1))
        {
            value = reinterpret_cast<function_t>(wglGetProcAddress(extName));
            PrintWglFunctionLoadInfo(value, extName);
        }
        return (value != nullptr);
    }
    inline _ReturnType operator()(_ArgsType ... args) const
    {
        return (value != nullptr) ? value(args ...) : _ReturnType();
    }
};

static auto __wglGetExtensionsStringARB  = FunctionProxy<PFNWGLGETEXTENSIONSSTRINGARBPROC>();
static auto __wglCreateContextAttribsARB = FunctionProxy<PFNWGLCREATECONTEXTATTRIBSARBPROC>();
static auto __wglChoosePixelFormatARB    = FunctionProxy<PFNWGLCHOOSEPIXELFORMATARBPROC>();
static auto __wglSwapIntervalEXT         = FunctionProxy<PFNWGLSWAPINTERVALEXTPROC>();
static auto __wglGetSwapIntervalEXT      = FunctionProxy<PFNWGLGETSWAPINTERVALEXTPROC>();

#include "extern_c/begin"

BOOL WINAPI _LoadExtensions()
{
    autox retval = true;
    log::debug(log::color::green)("info: load wgl extensions")();
#define __KV_GET_PROC_ADDR(extName)  do { retval &= __##extName.load(#extName); } while(0)
    __KV_GET_PROC_ADDR(wglGetExtensionsStringARB );
    __KV_GET_PROC_ADDR(wglCreateContextAttribsARB);
    __KV_GET_PROC_ADDR(wglChoosePixelFormatARB   );
    __KV_GET_PROC_ADDR(wglSwapIntervalEXT        );
    __KV_GET_PROC_ADDR(wglGetSwapIntervalEXT);
#undef __KV_GET_PROC_ADDR
    return retval;
}

static LRESULT WINAPI OnEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
#if 1
    log::debug.time()
        (log::color::cyan)("{0:^20} ", utils::GetWindowMessageName(message))
        (log::color::green)("(0x{0:04x}) ", message)
        (log::color::magenta)("wp:0x{0:016x} lp:0x{1:016x} ", wparam, lparam)();
#endif
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

    TCHAR class_name[] = TEXT("__kvTemporaryWindowWGL__");
    autox instance     = GetModuleHandle(nullptr);
    autox wcex         = WNDCLASSEX{ 0 };
    wcex.cbSize        = sizeof(wcex);
    wcex.hInstance     = instance;
    wcex.lpfnWndProc   = &OnEvent;
    wcex.lpszClassName = class_name;

    if (!RegisterClassEx(&wcex))
    {
        log::error("error: register window class failed.")();
        dispsose(0, nullptr, nullptr, nullptr, nullptr, nullptr);
        return FALSE;
    }

    autox hwnd = CreateWindowEx(WS_EX_APPWINDOW, class_name, nullptr, WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0, nullptr, nullptr, instance, nullptr);
    if (hwnd == nullptr)
    {
        log::error("error: create window instance failed")();
        dispsose(1, class_name, instance, nullptr, nullptr, nullptr);
        return FALSE;
    }

    autox hdc = GetDC(hwnd);
    if (hdc == nullptr)
    {
        log::error("error: get device handle failed.")();
        dispsose(2, class_name, instance, hwnd, nullptr, nullptr);
        return FALSE;
    }

    autox pfd = PIXELFORMATDESCRIPTOR{ 0 };
    if (!SetPixelFormat(hdc, 1, &pfd))
    {
        log::error("error: set pixel format failed.")();
        dispsose(3, class_name, instance, hwnd, hdc, nullptr);
        return FALSE;
    }

    autox hglrc = wgl::CreateContext(hdc);
    if (hglrc == nullptr)
    {
        log::error("error: create rendering context failed.")();
        dispsose(3, class_name, instance, hwnd, hdc, nullptr);
        return FALSE;
    }

    if (!wgl::MakeCurrent(hdc, hglrc))
    {
        log::error("error: make current context failed.")();
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
    autox extensions = GetExtensionsString(hdc);
    autox position   = strstr(extensions, extName);
    autox supported  = (position != nullptr);
    return supported;
}

LPCSTR WINAPI GetExtensionsString(HDC hdc)
{
    return __wglGetExtensionsStringARB(hdc);
}

HGLRC WINAPI CreateContextAttribs(HDC hdc, HGLRC hShareContext, const int * attribList)
{
    return __wglCreateContextAttribsARB(hdc, hShareContext, attribList);
}

BOOL WINAPI ChoosePixelFormat(HDC hdc, const int *piAttribIList, const float *pfAttribFList, int nMaxFormats, int *piFormats, int *nNumFormats)
{
    return __wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, reinterpret_cast<UINT *>(nNumFormats));
}

BOOL WINAPI SwapInterval(int interval)
{
    return __wglSwapIntervalEXT(interval);
}

int WINAPI GetSwapInterval(void)
{
    return __wglGetSwapIntervalEXT();
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
#include "extern_c/end"

}
#include "../_namespace/end"