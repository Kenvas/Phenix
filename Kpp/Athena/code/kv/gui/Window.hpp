#pragma once

#include "kv/common/Size.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <chrono>

#include "_namespace/begin"

class Window
{
private:

    using ClockType = std::chrono::high_resolution_clock;
    using TimePoint = ClockType::time_point;

    Size2i    Size_;
    HWND      WindowHandle_;
    TimePoint StartupTime_;

public:

    Window() noexcept;

    virtual ~Window() noexcept;

    Size2i const & GetSize() const noexcept;
    void SetSize(int width, int height) noexcept;

    bool Initialize();
    int Run();

protected:

    virtual void DoUpdate();
    virtual void DoRender();

    virtual bool OnCreate();
    virtual void OnDestroy();
    virtual void OnClose();
    virtual void OnFocus(bool enable);
    virtual void OnSize(int width, int height);

    HWND GetWindowHandle() const noexcept;

    virtual PCTSTR GetWindowClassName() const noexcept;

    virtual HWND CreateWindowInstance();

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam);
};

#include "_namespace/end"