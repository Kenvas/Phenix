#pragma once

#include <minwindef.h>

#include "kv/common/kvSize.hpp"

#include "_namespace/begin"

class Window
{
private:

    Size2i  Size_;
    HWND    WindowHandle_;
    int64_t StartupTime_;

public:

    Window() noexcept;

    virtual ~Window() noexcept;

    Size2i const & GetSize() const noexcept;
    void SetSize(int width, int height) noexcept;

    float GetTimeSinceStartup() const noexcept;

    bool Initialize();
    int Run();

protected:

    virtual void DoUpdate();
    virtual void DoRender();

    virtual bool OnCreate();
    virtual void OnDestroy();
    virtual void OnClose();
    virtual void OnFocus(bool enable);
    virtual void OnResize(int width, int height);

    HWND GetWindowHandle() const noexcept;

    virtual PCTSTR GetWindowClassName() const noexcept;

    virtual HWND CreateWindowInstance();

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam);
};

#include "_namespace/end"