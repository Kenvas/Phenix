#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"

#include "kv/predef/Char.hpp"

#include "kv/gui/kvOpenGLWindow.hpp"
#include "kv/common/kvRandom.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace kv;
using namespace kv::gui;

class glShader
{
protected:
    GLuint Id_;
    glShader(GLuint id)
        : Id_(id)
    {}
public:

    glShader(glShader && that)
        : Id_(that.Id_)
    {
        that.Id_ = 0;
    }

    ~glShader()
    {
        if (Id_ != 0)
        {
            log::info("delete shader: id = {0}", Id_)();
            glDeleteShader(Id_);
        }
    }

    static glShader CraeteFromFile(GLenum shader_type, cchar8s const filepath)
    {
        autox fs = ifstream(filepath);
        if (!fs.is_open())
        {
            log::error("failed to open: {0}", filepath)();
            return glShader(0);
        }
        fs.seekg(0, ios::end);
        autox size = fs.tellg();
        autox buffer = string();
        buffer.resize(size);
        fs.seekg(0);
        fs.read(const_cast<char *>(buffer.data()), size);
        autox shader = CreateFromString(shader_type, buffer.data());
        return shader;
    }

    static glShader CreateFromString(GLenum shader_type, GLchar const * const code)
    {
        if (code == nullptr)
            return glShader(0);
        autox id = glCreateShader(shader_type);
        glShaderSource(id, 1, &code, nullptr);
        glCompileShader(id);
        autox result = GLint{};
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            GLchar info[512];
            glGetShaderInfoLog(id, sizeof(info), nullptr, info);
            log::error("error: shader compiles error. {0}", info)();
            glDeleteShader(id);
            return glShader(0);
        }
        return glShader(id);
    }
};

class glProgram
{
protected:
    GLuint Id_;
};

class OpenGLExample : public OpenGLWindow
{
private:
    GLfloat TriangleData[9];
    GLuint vbo[1];

public:
    virtual ~OpenGLExample() override
    {

    }
private:
    virtual bool OnCreate() override
    {
        if (!OpenGLWindow::OnCreate())
            return false;

        glClearColor(0.5, 0.5, 1, 1);
        autox size = GetSize();
        glViewport(0, 0, size.width, size.height);

        TriangleData[0] = -0.4f;
        TriangleData[1] = 0.1f;
        TriangleData[2] = 0.0f;
        TriangleData[3] = 0.4f;
        TriangleData[4] = 0.1f;
        TriangleData[5] = 0.0f;
        TriangleData[6] = 0.0f;
        TriangleData[7] = 0.7f;
        TriangleData[8] = 0.0f;

        glGenBuffers(sizeof(vbo)/sizeof(GLuint), vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleData), TriangleData, GL_STATIC_DRAW);
        autox render_vs = glShader::CraeteFromFile(GL_VERTEX_SHADER, "render_vs.glsl");
        autox render_fs = glShader::CraeteFromFile(GL_FRAGMENT_SHADER, "render_fs.glsl");
        return true;
    }

    virtual void OnDestroy() override
    {
        glDeleteBuffers(sizeof(vbo) / sizeof(GLuint), vbo);
        OpenGLWindow::OnDestroy();
    }

    virtual void DoRender() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        SwapBuffers();
        glFlush();
    }
};

KV_QuickAddEntry
{
    autox app = make_shared<OpenGLWindow>();

    app->SetSize(800, 600);
    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};