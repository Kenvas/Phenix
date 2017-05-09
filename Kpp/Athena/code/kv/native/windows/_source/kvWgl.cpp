#include "PrecompiledHeader.hpp"

#include "../kvWgl.hpp"
#include "../kvUtils.hpp"
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
        utils::PrintErrorMessageInfo();
    }
    else
    {
        log::debug(log::color::magenta)("    {0:<26}", extName)(log::color::green)(" success.")();
    }
}

//template <class _FunctionType>
//struct FunctionProxy;
//
//template <class _ReturnType, class ... _ArgsType>
//struct FunctionProxy<_ReturnType(WINAPI *)(_ArgsType ...)>
//{
//private:
//    using function_t = _ReturnType(WINAPI *)(_ArgsType ...);
//    function_t value;
//public:
//    FunctionProxy() : value(function_t(-1)) {}
//    inline bool load(LPCSTR const extName) noexcept
//    {
//        if (value == function_t(-1))
//        {
//            value = reinterpret_cast<function_t>(wglGetProcAddress(extName));
//            PrintWglFunctionLoadInfo(reinterpret_cast<void *>(value), extName);
//        }
//        return (value != nullptr);
//    }
//    inline _ReturnType operator()(_ArgsType ... args) const
//    {
//        return (value != nullptr) ? value(args ...) : _ReturnType();
//    }
//};
//
//static auto __wglGetExtensionsStringARB  = FunctionProxy<PFNWGLGETEXTENSIONSSTRINGARBPROC>();
//static auto __wglCreateContextAttribsARB = FunctionProxy<PFNWGLCREATECONTEXTATTRIBSARBPROC>();
//static auto __wglChoosePixelFormatARB    = FunctionProxy<PFNWGLCHOOSEPIXELFORMATARBPROC>();
//static auto __wglSwapIntervalEXT         = FunctionProxy<PFNWGLSWAPINTERVALEXTPROC>();
//static auto __wglGetSwapIntervalEXT      = FunctionProxy<PFNWGLGETSWAPINTERVALEXTPROC>();
//
//BOOL WINAPI _LoadExtensions()
//{
//    autox retval = true;
//    log::debug(log::color::green)("info: load wgl extensions")();
//#define __KV_GET_PROC_ADDR(extName)  do { retval &= __##extName.load(#extName); } while(0)
//    __KV_GET_PROC_ADDR(wglGetExtensionsStringARB );
//    __KV_GET_PROC_ADDR(wglCreateContextAttribsARB);
//    __KV_GET_PROC_ADDR(wglChoosePixelFormatARB   );
//    __KV_GET_PROC_ADDR(wglSwapIntervalEXT        );
//    __KV_GET_PROC_ADDR(wglGetSwapIntervalEXT     );
//#undef __KV_GET_PROC_ADDR
//    return retval;
//}

static LRESULT WINAPI OnWindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
#if 1
    utils::PrintWindowMessageInfo(hwnd, message, wparam, lparam);
#endif
    return DefWindowProc(hwnd, message, wparam, lparam);
}

bool LoadExtensions()
{
    static autox init = false;

    if (init) return true;

    autox dispsose = [](int step, LPCTSTR class_name, HINSTANCE instance, HWND hwnd, HDC hdc, HGLRC hglrc)
    {
        switch (step)
        {
        case 5: wglMakeCurrent(nullptr, nullptr);
        case 4: wglDeleteContext(hglrc);
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
    wcex.lpfnWndProc   = &OnWindowProcedure;
    wcex.lpszClassName = class_name;

    log::debug()("********** temporary window begin **********")();

    if (!RegisterClassEx(&wcex))
    {
        log::error("error: register window class failed.")();
        dispsose(0, nullptr, nullptr, nullptr, nullptr, nullptr);
        return false;
    }

    autox hwnd = CreateWindowEx(WS_EX_APPWINDOW, class_name, nullptr, WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0, nullptr, nullptr, instance, nullptr);
    if (hwnd == nullptr)
    {
        log::error("error: create window instance failed")();
        dispsose(1, class_name, instance, nullptr, nullptr, nullptr);
        return false;
    }

    autox hdc = GetDC(hwnd);
    if (hdc == nullptr)
    {
        log::error("error: get device handle failed.")();
        dispsose(2, class_name, instance, hwnd, nullptr, nullptr);
        return false;
    }

    autox pfd = PIXELFORMATDESCRIPTOR{ 0 };
    if (!SetPixelFormat(hdc, 1, &pfd))
    {
        log::error("error: set pixel format failed.")();
        dispsose(3, class_name, instance, hwnd, hdc, nullptr);
        return false;
    }

    autox hglrc = wglCreateContext(hdc);
    if (hglrc == nullptr)
    {
        log::error("error: create rendering context failed.")();
        dispsose(3, class_name, instance, hwnd, hdc, nullptr);
        return false;
    }

    if (!wglMakeCurrent(hdc, hglrc))
    {
        log::error("error: make current context failed.")();
        dispsose(4, class_name, instance, hwnd, hdc, hglrc);
        return false;
    }

    //_LoadExtensions();
    autox result = glewInit();
    if (result != GLEW_OK)
    {
        log::error("error: initialize glew failed.")();
    }
    else
    {
        SetLastError(ERROR_SUCCESS);
    }

    dispsose(5, class_name, instance, hwnd, hdc, hglrc);

    log::debug("********** temporary window end **********")()();

    init = true;

    return init;
}
//
//BOOL WINAPI IsExtensionSupported(HDC hdc, LPCSTR extName)
//{
//    autox extensions = GetExtensionsString(hdc);
//    autox position   = strstr(extensions, extName);
//    autox supported  = (position != nullptr);
//    return supported;
//}
//
//LPCSTR WINAPI GetExtensionsString(HDC hdc)
//{
//    return __wglGetExtensionsStringARB(hdc);
//}
//
//HGLRC WINAPI CreateContextAttribs(HDC hdc, HGLRC hShareContext, const int * attribList)
//{
//    return __wglCreateContextAttribsARB(hdc, hShareContext, attribList);
//}
//
//BOOL WINAPI ChoosePixelFormat(HDC hdc, const int *piAttribIList, const float *pfAttribFList, int nMaxFormats, int *piFormats, int *nNumFormats)
//{
//    return __wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, reinterpret_cast<UINT *>(nNumFormats));
//}
//
//BOOL WINAPI SwapInterval(int interval)
//{
//    return __wglSwapIntervalEXT(interval);
//}
//
//int WINAPI GetSwapInterval(void)
//{
//    return __wglGetSwapIntervalEXT();
//}
//
//HGLRC WINAPI CreateContext(HDC hdc)
//{
//    return wglCreateContext(hdc);
//}
//
//BOOL WINAPI DeleteContext(HGLRC hglrc)
//{
//    return wglDeleteContext(hglrc);
//}
//
//BOOL WINAPI MakeCurrent(HDC hdc, HGLRC hglrc)
//{
//    return wglMakeCurrent(hdc, hglrc);
//}

}
#include "../_namespace/end"