#pragma once

#include "kvWindow.hpp"

#include "_namespace/begin"
class OpenGLWindow : public Window
{
protected:
    HDC   DeviceContextHandle_;
    HGLRC RenderingContextHandle_;

public:
    using Window::Window;

    virtual ~OpenGLWindow() noexcept override;

protected:

    bool MakeCurrent() const;

    bool SwapBuffers() const;

    bool SetupPixelFormat();

    bool CreateRenderingContext(int const major, int const minor);

    virtual bool OnCreate() override;

    virtual void OnDestroy() override;

    virtual LRESULT CALLBACK OnEvent(UINT message, WPARAM wparam, LPARAM lparam) override;

    virtual void DoRender() override;
};
#include "_namespace/end"