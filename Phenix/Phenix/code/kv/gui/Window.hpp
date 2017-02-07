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

    size2i    Size_;
    HWND      WindowHandle_;
    TimePoint StartupTime_;

public:

    Window() noexcept;

    virtual ~Window() noexcept;

    size2i const & GetSize() const noexcept;
    void SetSize(int const width, int const height) noexcept;

    bool Initialize();
    int Run();

protected:

    virtual void OnInput();
    virtual void OnUpdate();
    virtual void OnRender();

    virtual bool OnCreate();
    virtual void OnDestroy();
    virtual void OnClose();
    virtual void OnSize(int const width, int const height);

    HWND GetWindowHandle() const noexcept;

    virtual PCTSTR GetWindowClassName() const noexcept;

    virtual HWND CreateWindowInstance(PCTSTR const class_name, PCTSTR const window_title);

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam);
};

#include "_namespace/end"