#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <chrono>

#include "kv/predef/Keyword.hpp"
#include "kv/common/Size.hpp"

#include "_namespace/begin"

class Window
{
private:

    using ClockType = std::chrono::high_resolution_clock;
    using TimePoint = ClockType::time_point;

    Size      Size_;
    HWND      WindowHandle_;
    TimePoint StartupTime_;

public:

    Window() noexcept;
    Window(int width, int height) noexcept;

    virtual ~Window() noexcept;

    Size const & GetSize() const noexcept;
    void SetSize(int width, int height);

    int Run() const;
    bool Initialize();

    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnClose();
    virtual void OnResize(int width, int height);

protected:

    HWND const GetWindowHandle() const noexcept;

    virtual PCTSTR const GetWindowClassName() const noexcept;
    virtual PCTSTR const GetWindowTitle() const noexcept;

    virtual bool InitializeDetail();
    virtual bool RegisterWindowClass(PCTSTR const class_name, WNDPROC const window_procdure) const noexcept;
            void UnregisterWindowClass(PCTSTR const class_name) const noexcept;
    virtual HWND CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_title);

    virtual LRESULT CALLBACK OnEvent
    (
        UINT   message,
        WPARAM wparam,
        LPARAM lparam
    );

    static LRESULT CALLBACK ForwardWindowProcedure
    (
        HWND   window_handle,
        UINT   message,
        WPARAM wparam,
        LPARAM lparam
    );

    static LRESULT CALLBACK AuxWindowProcedure
    (
        HWND   window_handle,
        UINT   message,
        WPARAM wparam,
        LPARAM lparam
    );
};

#include "_namespace/end"