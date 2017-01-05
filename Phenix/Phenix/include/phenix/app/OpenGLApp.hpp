#pragma once

//#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

//#include "glbinding/gl/gl.h"
//#include "glbinding/Binding.h"

#include "WindowApp.hpp"

#include "detail/wgl.hpp"
#include "phenix/log/Log.hpp"


using namespace std;

namespace fnx
{
    namespace app
    {
        class OpenGLApp : public WindowApp
        {
        protected:
            HDC   DeviceContextHandle_;
            HGLRC RenderingContextHandle_;
        public:
            using WindowApp::WindowApp;

        protected:

            bool SetupPixelFormat()
            {
                int const iPixelFormatAttributes[] =
                {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE                  ,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE                  ,
                    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE                  ,
                    WGL_DOUBLE_BUFFER_ARB , GL_TRUE                  ,
                    WGL_ACCELERATION_ARB  , WGL_FULL_ACCELERATION_ARB,
                    WGL_SWAP_METHOD_ARB   , WGL_SWAP_EXCHANGE_ARB    ,
                    WGL_PIXEL_TYPE_ARB    , WGL_TYPE_RGBA_ARB        ,
                    WGL_COLOR_BITS_ARB    , 32                       ,
                    WGL_DEPTH_BITS_ARB    , 24                       ,
                    WGL_STENCIL_BITS_ARB  , 8                        ,
                    WGL_SAMPLES_ARB       , 4                        ,
                    0
                };

                int iPixelFormat;
                int iNumFormat;
                if (!wgl::ChoosePixelFormat(DeviceContextHandle_, iPixelFormatAttributes, nullptr, 1, &iPixelFormat, &iNumFormat))
                {
                    log::simple->error("error: wgl choose pixel format failed.");
                    return false;
                }
                log::simple->info(log::color::green, "info: wgl choose pixel format success.");

                autox pfd = PIXELFORMATDESCRIPTOR();
                if (!SetPixelFormat(DeviceContextHandle_, iPixelFormat, &pfd))
                {
                    log::simple->error("error: set pixel format (index = {0}) failed.", iPixelFormat);
                    return false;
                }
                log::simple->info(log::color::green, "info: set pixel format (index = {0}) success.", iPixelFormat);

                return true;
            }

            bool const CreateRenderingContext(int const major, int const minor)
            {
                int const iContextAttributes[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, major,
                    WGL_CONTEXT_MINOR_VERSION_ARB, minor,
#if defined(_DEBUG)
                    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#else
                    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#endif
                    0
                };

                RenderingContextHandle_ = wgl::CreateContextAttribs(DeviceContextHandle_, nullptr, iContextAttributes);
                auto errcode = glGetError();
                if (RenderingContextHandle_ == nullptr)
                {
                    log::simple->error("error: create rendering context (version {0}.{1}) failed.", major, minor);
                    return false;
                }
                log::simple->info(log::color::green, "info: create rendering context (version {0}.{1}) success.", major, minor);

                return true;
            }

            virtual bool InitializeDetail() override
            {
                if (!wgl::LoadExtensions())
                {
                    return false;
                }

                window_handle = GetWindowHandle();
                DeviceContextHandle_ = GetDC(window_handle);
                error = GetLastError();
                if (DeviceContextHandle_ == nullptr)
                {
                    log::simple->error("error: get device handle failed.");
                    return false;
                }

                if (!SetupPixelFormat())
                {
                    return false;
                }

                autox list = vector<pair<int, int>>
                {
                    make_pair(4, 5),
                    make_pair(4, 4),
                    make_pair(4, 3),
                    make_pair(4, 2),
                    make_pair(4, 1),
                    make_pair(4, 0),
                    make_pair(3, 3),
                    make_pair(3, 2),
                    make_pair(3, 1),
                    make_pair(3, 0),
                };

                autox found = false;
                for (autox v : list)
                {
                    found = CreateRenderingContext(v.first, v.second);
                    if (found) break;
                }
                if (!found)
                {
                    return false;
                }

                wgl::MakeCurrent(DeviceContextHandle_, RenderingContextHandle_);

                glEnable(GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);
                glCullFace(GL_BACK);
                glFrontFace(GL_CW);
                glClearDepth(1.0f);
                glClearColor(1, 0, 1, 1);

                return true;
            }

            virtual LRESULT CALLBACK WindowProcedure(UINT message, WPARAM wparam, LPARAM lparam) override
            {
                return WindowApp::WindowProcedure(message, wparam, lparam);
            }
        };
    }
}