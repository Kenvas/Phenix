#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"

#include "kv/predef/Char.hpp"

#include "kv/gui/kvOpenGLWindow.hpp"
#include "kv/common/kvRandom.hpp"

#include "FreeImage/FreeImagePlus.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

    operator GLuint()
    {
        return Id_;
    }

    operator GLuint const() const
    {
        return Id_;
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
        log::debug(log::color::green)("load shader({0}): ", shader_type)(log::color::cyan)("{0}", filepath)();
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
            log::error(info);
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
    glProgram(GLuint id)
        : Id_(id)
    {}
public:

    static glProgram Default;

    glProgram()
        : Id_(0)
    {}

    glProgram(glProgram && that)
        : Id_(that.Id_)
    {
        that.Id_ = 0;
    }

    ~glProgram()
    {
        //if (Id_ != 0)
        //{
        //    glDeleteProgram(Id_);
        //}
    }

    operator GLuint()
    {
        return Id_;
    }

    operator GLuint const() const
    {
        return Id_;
    }

    glProgram& operator=(glProgram const &) = default;

    static glProgram Create()
    {
        autox id = glCreateProgram();
        return glProgram(id);
    }

    void Delete()
    {
        if (Id_ != 0)
        {
            glDeleteProgram(Id_);
        }
    }

    glProgram& Attach(glShader const & shader)
    {
        glAttachShader(Id_, shader);
        return *this;
    }

    bool Link()
    {
        glLinkProgram(Id_);
        autox result = GLint();
        glGetProgramiv(Id_, GL_LINK_STATUS, &result);
        if (result != GL_TRUE)
        {
            GLchar info[512];
            glGetProgramInfoLog(Id_, sizeof(info), nullptr, info);
            log::error(info);
            glDeleteProgram(Id_);
            Id_ = 0;
            return false;
        }
        //glBindFragDataLocation(Id_, 0, "color");
        return true;
    }

    void Use() const
    {
        glUseProgram(Id_);
    }
};

glProgram glProgram::Default = glProgram(0);

class OpenGLExample : public OpenGLWindow
{
private:
    GLuint bo[2];
    GLuint vao[1];
    GLuint tex[2];
    glProgram prog;
    vector<GLfloat> VertextData;
    vector<GLuint> IndexData;
    GLint UnilocTime;
    GLint UnilocTex[2];
    fipImage TexImage[2];

public:

    OpenGLExample() noexcept
    {

    }

    virtual ~OpenGLExample() noexcept override
    {
        glDeleteVertexArrays(sizeof(vao) / sizeof(GLuint), vao);
        glDeleteBuffers(sizeof(bo) / sizeof(GLuint), bo);
    }
private:
    virtual bool OnCreate() override
    {
        if (!OpenGLWindow::OnCreate())
            return false;

        autox client = GetSize();
        OnResize(client.width, client.height);

        glClearDepth(1.0f);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glEnable(GL_CULL_FACE); // cull face
        //glCullFace(GL_BACK); // cull back face
        //glFrontFace(GL_CW); // GL_CCW for counter clock-wise

        VertextData = vector<GLfloat>
        {
             0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
             0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
            -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
            -0.75f,  0.75f, 0.0f, 0.0f, 1.0f,
        };

        IndexData = vector<GLuint>
        {
            0, 1, 3,
            1, 2, 3,
        };

        glGenBuffers(sizeof(bo)/sizeof(GLuint), bo);
        glGenVertexArrays(sizeof(vao)/sizeof(GLuint), vao);
        glBindVertexArray(vao[0]);
            glBindBuffer(GL_ARRAY_BUFFER, bo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VertextData.size(), VertextData.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IndexData.size(),IndexData.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(0));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        autox render_vs = glShader::CraeteFromFile(GL_VERTEX_SHADER, "render_vs.glsl");
        autox render_fs = glShader::CraeteFromFile(GL_FRAGMENT_SHADER, "render_fs.glsl");
        prog = glProgram::Create()
            .Attach(render_vs)
            .Attach(render_fs);
        prog.Link();
        prog.Use();
        UnilocTime = glGetUniformLocation(prog, "time");
        UnilocTex[0] = glGetUniformLocation(prog, "texImage0");
        UnilocTex[1] = glGetUniformLocation(prog, "texImage1");

        TexImage[0].load("test.png");
        TexImage[1].load("face.png");

        glGenTextures(sizeof(tex)/sizeof(GLuint), tex);

        // create and set texture parameter for 'text.png'
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(UnilocTex[0], 0);
            glBindTexture(GL_TEXTURE_2D, tex[0]);
            auto width = TexImage[0].getWidth();
            auto height = TexImage[0].getHeight();
            auto pixels = TexImage[0].accessPixels();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GLfloat border_color[] = {1.0f, 1.0f, 0.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // create and set texture parameter for 'face.png'
        {
            glActiveTexture(GL_TEXTURE0 + 1);
            glUniform1i(UnilocTex[1], 1);
            glBindTexture(GL_TEXTURE_2D, tex[1]);
            auto width = TexImage[1].getWidth();
            auto height = TexImage[1].getHeight();
            auto pixels = TexImage[1].accessPixels();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GLfloat border_color[] = {1.0f, 1.0f, 0.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        return true;
    }

    virtual void OnDestroy() override
    {
        glDeleteBuffers(sizeof(bo) / sizeof(GLuint), bo);
        OpenGLWindow::OnDestroy();
    }

    virtual void OnResize(int width, int height) override
    {
        OpenGLWindow::OnResize(width, height);
        glViewport(0, 0, width, height);
        DoRender();
    }

    virtual void DoRender() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, tex[1]);
        glBindVertexArray(vao[0]);
        //glDrawArrays(GL_TRIANGLES, 0, IndexData.size());
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[1]);
        auto time = GetTimeSinceStartup();
        glUniform1f(UnilocTime, time);
        glDrawElements(GL_TRIANGLES, GLsizei(IndexData.size()), GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, IndexData.size(), GL_UNSIGNED_INT, IndexData.data());
        glBindVertexArray(0);

        SwapBuffers();
    }
};

KV_QuickAddEntry
{
    autox app = make_shared<OpenGLExample>();

    int64_t tkcount = chrono::high_resolution_clock::now().time_since_epoch().count();

    app->SetSize(800, 600);
    if (app->Initialize())
    {
        app->Run();
    }

    return 0;
};