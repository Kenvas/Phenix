#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"

#include "kv/predef/Char.hpp"

#include "kv/gui/kvOpenGLWindow.hpp"
#include "kv/common/kvRandom.hpp"

#include "FreeImage/FreeImagePlus.h"
#include "Eigen/Core"
#include "Eigen/Geometry"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace kv;
using namespace kv::gui;

using namespace Eigen;

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
    GLint UnilocModelMatrix;
    GLint UnilocViewMatrix;
    GLint UnilocProjMatrix;
    fipImage TexImage[2];

    vector<Vector3f> LocationData;

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


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glClearDepth(1.0f);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glEnable(GL_CULL_FACE); // cull face
        //glCullFace(GL_BACK); // cull back face
        //glFrontFace(GL_CW); // GL_CCW for counter clock-wise

        auto a = 0.75f;

        VertextData = vector<GLfloat>
        {
             -a, -a,  a, 0.0f, 0.0f,
              a, -a,  a, 1.0f, 0.0f,
              a,  a,  a, 1.0f, 1.0f,
             -a,  a,  a, 0.0f, 1.0f,

              a, -a,  a, 0.0f, 0.0f,
              a, -a, -a, 1.0f, 0.0f,
              a,  a, -a, 1.0f, 1.0f,
              a,  a,  a, 0.0f, 1.0f,

             -a,  a,  a, 0.0f, 0.0f,
              a,  a,  a, 1.0f, 0.0f,
              a,  a, -a, 1.0f, 1.0f,
             -a,  a, -a, 0.0f, 1.0f,

             -a, -a,  a, 0.0f, 0.0f,
             -a, -a, -a, 1.0f, 0.0f,
             -a,  a, -a, 1.0f, 1.0f,
             -a,  a,  a, 0.0f, 1.0f,

             -a, -a,  a, 0.0f, 0.0f,
              a, -a,  a, 1.0f, 0.0f,
              a, -a, -a, 1.0f, 1.0f,
             -a, -a, -a, 0.0f, 1.0f,

             -a, -a,  -a, 0.0f, 0.0f,
              a, -a,  -a, 1.0f, 0.0f,
              a,  a,  -a, 1.0f, 1.0f,
             -a,  a,  -a, 0.0f, 1.0f,
        };

        IndexData = vector<GLuint>
        {
            0, 1, 2,
            0, 2, 3,

            0 + 4, 1 + 4, 2 + 4,
            0 + 4, 2 + 4, 3 + 4,

            0 + 8, 1 + 8, 2 + 8,
            0 + 8, 2 + 8, 3 + 8,

            0 + 12, 1 + 12, 2 + 12,
            0 + 12, 2 + 12, 3 + 12,

            0 + 16, 1 + 16, 2 + 16,
            0 + 16, 2 + 16, 3 + 16,

            0 + 20, 1 + 20, 2 + 20,
            0 + 20, 2 + 20, 3 + 20,
        };

        LocationData = vector<Vector3f>
        {
            { 0.0f,  0.0f,  0.0f },
            { 2.0f,  5.0f, -15.0f},
            {-1.5f, -2.2f, -2.5f },
            {-3.8f, -2.0f, -12.3f},
            { 2.4f, -0.4f, -3.5f },
            {-1.7f,  3.0f, -7.5f },
            { 1.3f, -2.0f, -2.5f },
            { 1.5f,  2.0f, -2.5f },
            { 1.5f,  0.2f, -1.5f },
            {-1.3f,  1.0f, -1.5f }
        };


        glGenBuffers(sizeof(bo)/sizeof(GLuint), bo);
        glBindBuffer(GL_ARRAY_BUFFER, bo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(VertextData)::value_type) * VertextData.size(), VertextData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(VertextData)::value_type) * IndexData.size(),IndexData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(sizeof(vao)/sizeof(GLuint), vao);
        glBindVertexArray(vao[0]);
            glBindBuffer(GL_ARRAY_BUFFER, bo[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(0));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[1]);
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

        UnilocTime        = glGetUniformLocation(prog, "time");
        UnilocTex[0]      = glGetUniformLocation(prog, "texImage0");
        UnilocTex[1]      = glGetUniformLocation(prog, "texImage1");
        UnilocModelMatrix = glGetUniformLocation(prog, "modelMat");
        UnilocViewMatrix  = glGetUniformLocation(prog, "viewMat");
        UnilocProjMatrix  = glGetUniformLocation(prog, "projMat");

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
        auto count = LocationData.size();
        for (auto i = 0; i < count; i++)
        {
            auto time = GetTimeSinceStartup();
            glUniform1f(UnilocTime, time + i * i);
            auto location = LocationData[i];
            auto modelMat = Affine3f(UniformScaling<float>(1))
                .translate(!location.isZero() ? location.normalized() : Vector3f(0.5, 0.5, 0))
                .rotate(AngleAxisf(0.25f * 3.1415926f * time * (i+1), (Vector3f(1, 2, 3) + location).normalized()))
                .scale(0.2f)
                ;
            glUniformMatrix4fv(UnilocModelMatrix, 1, GL_FALSE, modelMat.data());
            glDrawElements(GL_TRIANGLES, GLsizei(IndexData.size()), GL_UNSIGNED_INT, nullptr);
        }
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