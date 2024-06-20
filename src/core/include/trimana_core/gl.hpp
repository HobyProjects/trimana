#pragma once

#include <fstream>
#include <string>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SDL3/SDL.h>

#include "logger.hpp"

#define GL_DEFAULT_GENERATE_BUFFERS 1
#define GL_UNBIND 0

typedef unsigned int ShaderProgramLoc;
typedef unsigned int VertexShader;
typedef unsigned int FragmentShader;
typedef unsigned int VertexBufferLoc;
typedef unsigned int VertexAttributePtr;
typedef unsigned int ElementBufferLoc;
typedef unsigned int *ElementBufferData;
typedef float *VertexBufferData;
typedef unsigned int TextureLocation;
typedef unsigned int UniformVarLoc;
typedef unsigned int FrameBufferLoc;
typedef unsigned int DepthBufferLoc;

namespace TrimanaCore
{
    class GL
    {
    private:
        GL() = default;
        ~GL() = default;

    public:
        static bool GL_Load();
        static std::string GetVersion() { return mGLVersion; }
        static std::string GetVendor() { return mGLVendor; }
        static std::string GetRenderer() { return mGLRenderer; }
        static std::string GetGLSLVersion() { return mGLSLVersion; }
        static bool IsLoadSuccess() { return GL_LoadSuccess; }

    public:
        static std::string mGLVersion;
        static std::string mGLVendor;
        static std::string mGLRenderer;
        static std::string mGLSLVersion;
        static bool GL_LoadSuccess;
    };

    enum class SHADER_TYPE
    {
        SHADER_PROGRAM = 0x0,
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER
    };

    enum class VERTEX_BUFFER_TYPE
    {
        VERTEX_BUFFER = 0x1,
        COLOR_BUFFER = 0x2,
        TEXTURE_BUFFER = 0x3,
        NORMALS_BUFFER = 0x4
    };

    enum class ELEMENT_BUFFER_TYPE
    {
        INDICES_BUFFER = 0x1
    };

    enum class COMPONENT_TYPE
    {
        COMPONENTS_XY = 0x2,
        COMPONENTS_XYZ = 0x3,
        COMPONENTS_RGB = 0x3,
        COMPONENTS_RGBA = 0x4,
        COMPONENTS_UV = 0x2
    };

    enum class DATA_TYPE
    {
        FLOAT_TY = GL_FLOAT,
        UINT_TY = GL_UNSIGNED_INT,
        INT_TY = GL_INT
    };

    enum class DRAW_TYPE
    {
        DRAW_STATIC = GL_STATIC_DRAW,
        DRAW_DYNAMIC = GL_DYNAMIC_DRAW
    };

    enum class DRAW_CALLS
    {
        DRAW_POINTS = GL_POINTS,
        DRAW_TRIANGLES = GL_TRIANGLES,
        DRAW_LINES = GL_LINES
    };

    enum class COLOR_CHANNELS
    {
        RGB = 0x3,
        RGBA = 0x4
    };

    class Shader
    {
    public:
        Shader(const std::string &vshader, const std::string &fshader);
        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        ~Shader();

        UniformVarLoc GetUniformLoc(SHADER_TYPE program, const std::string &uniform_val);
        void ShaderAttach();
        void ShaderDettach();

        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, int data);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, unsigned int data);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float data);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y, float z);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, glm::mat4 &data);
        bool UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, glm::mat3 &data);

    private:
        UniformVarLoc UniformUpdateValidation(SHADER_TYPE type, const std::string &uniform_var);
        ShaderProgramLoc CompileShaderProgram(ShaderProgramLoc &program, const std::string &shader_code, TrimanaCore::SHADER_TYPE shader_type);
        std::string ImportShader(const std::string &shader_file);

    public:
        std::string VertexShaderCode;
        std::string FragmentShaderCode;

        ShaderProgramLoc ShderProgramSelf{NULL};
        VertexShader VertexShaderProgram{NULL};
        FragmentShader FragmentShaderProgram{NULL};
        bool ShaderProgramCreated{false};
    };

    class VertexBuffers
    {
    public:
        VertexBuffers(unsigned int num_buffers);
        VertexBuffers(const VertexBuffers &) = delete;
        VertexBuffers &operator=(const VertexBuffers &) = delete;
        ~VertexBuffers();

        void AssignVertexBufferData(VERTEX_BUFFER_TYPE vtype, VertexBufferData data, GLsizeiptr size, DRAW_TYPE dtype);
        void AssignElementBufferData(ElementBufferData data, GLsizeiptr size, DRAW_TYPE dtype);
        void LinkVertexBuffers(ShaderProgramLoc &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
        void LinkUsingBufferLayout(unsigned int layout, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
        void LinkElementBuffers();
        void Render(DRAW_CALLS dcall);

    private:
        VertexBufferLoc VertexBuff{NULL};
        VertexBufferLoc ColorBuff{NULL};
        VertexBufferLoc TextureBuff{NULL};
        VertexBufferLoc NormalsBuff{NULL};
        ElementBufferLoc ElementBuff{NULL};
        VertexAttributePtr VertexArryPtr{NULL};

        unsigned int IndicesCount{NULL};
        unsigned int NumOfBuffers{NULL};
    };

    TextureLocation LoadTexture(const std::string &loc, COLOR_CHANNELS channels, bool flip = true);
    void TextureAttach(TextureLocation &texture);
    void TextureDettach();
    void DeleteTexture(TextureLocation &texture);
};