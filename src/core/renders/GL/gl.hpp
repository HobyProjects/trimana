#pragma once

#include <fstream>
#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "utils/logger.hpp"
#define GL_UNBIND 0

typedef unsigned int ShaderProgram;
typedef unsigned int VertexShader;
typedef unsigned int FragmentShader;
typedef unsigned int VertexBufferLoc;
typedef unsigned int VertexAttributePtr;
typedef unsigned int ElementBufferLoc;
typedef unsigned int *ElementBufferData;
typedef float        *VertexBufferData;
typedef unsigned int TextureLocation;
typedef unsigned int UniformVarLoc;
typedef unsigned int FrameBufferLoc;
typedef unsigned int DepthBufferLoc;

namespace TrimanaCore
{
    struct GLInfo
    {
        std::string GLVersion{"Unknown"};
        std::string GLVendor{"Unknown"};
        std::string GLRenderer{"Unknown"};
        std::string GLSLVersion{"#versio ?.?.?"};
        bool GL_LoadSuccess{false};
    };

    enum class SHADER_TYPE
    {
        VERTEX_SHADER   = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER
    };

    enum class VERTEX_BUFFER_TYPE
    {
        VERTEX_BUFFER   = 0x1,
        COLOR_BUFFER    = 0x2,
        TEXTURE_BUFFER  = 0x3,
        NORMALS_BUFFER  = 0x4
    };

    enum class ELEMENT_BUFFER_TYPE
    {
        INDICES_BUFFER = 0x1
    };

    enum class COMPONENT_TYPE
    {
        COM_TY_XY       = 0x2,
        COM_TY_XYZ      = 0x3,
        COM_TY_RGB      = 0x3,
        COM_TY_RGBA     = 0x4,
        COM_TY_UV       = 0x2
    };

    enum class DATA_TYPE
    {
        FLOAT_TY    = GL_FLOAT,
        UINT_TY     = GL_UNSIGNED_INT,
        INT_TY      = GL_INT
    };

    enum class DRAW_TYPE
    {
        DRAW_STATIC     = GL_STATIC_DRAW,
        DRAW_DYNAMIC    = GL_DYNAMIC_DRAW
    };

    enum class DRAW_CALLS
    {
        DRAW_POINTS         = GL_POINTS,
        DRAW_TRIANGLES      = GL_TRIANGLES,
        DRAW_LINES          = GL_LINES
    };

    enum class COLOR_CHANNELS
    {
        RGB     = 0x3,
        RGBA    = 0x4
    };

    class TRIMANA_CORE Shader
    {
    public:
        Shader() = default;
        ~Shader() = default;

        ShaderProgram ShderProgramSelf{NULL};
        VertexShader VertexShaderProgram{NULL};
        FragmentShader FragmentShaderProgram{NULL};

        std::string VertexShaderCode{"undefine"};
        std::string FragmentShaderCode{"undefine"};

        bool ShaderProgramCreated{false};
    };

    class TRIMANA_CORE VertexBuffers
    {
    public:
        VertexBuffers() = default;
        ~VertexBuffers() = default;

        VertexBufferLoc VertexBuff{NULL};
        VertexBufferLoc ColorBuff{NULL};
        VertexBufferLoc TextureBuff{NULL};
        VertexBufferLoc NormalsBuff{NULL};
        ElementBufferLoc element_buff{NULL};
        VertexAttributePtr VertexArryPtr{NULL};
        unsigned int IndicesCount{NULL};
    };

    VertexBuffers *GenerateBuffers(unsigned int num_buffers);
    void AssignVertexBufferData(VertexBuffers *buffers, VERTEX_BUFFER_TYPE vtype, VertexBufferData data, GLsizeiptr size, DRAW_TYPE dtype);
    void AssignElementBufferData(VertexBuffers *buffers, ElementBufferData data, GLsizeiptr size, DRAW_TYPE dtype);
    void LinkVertexBuffers(VertexBuffers *buffers, ShaderProgram &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
    void LinkBufferLayout(unsigned int layout, VertexBuffers *buffers, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
    void LinkElementBuffers(VertexBuffers *buffer);
    void Render(VertexBuffers *buffers, DRAW_CALLS dcall);
    void DeleteBuffers(VertexBuffers *buffers, unsigned int num_buffers_used);

    UniformVarLoc GetUniformLoc(ShaderProgram &program, const std::string &uniform_val);
    Shader *CreateShaderProgram(const std::string &vshader, const std::string &fshader);
    void ShaderAttach(ShaderProgram &program);
    void ShaderDettach();
    bool DeleteShaders_N_Programs(Shader* programs);

    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, int data);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, unsigned int data);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float data);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat4 &data);
    bool UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat3 &data);

    TextureLocation LoadTexture(const std::string &loc, COLOR_CHANNELS channels, bool flip = true);
    void TextureAttach(TextureLocation &texture);
    void TextureDettach();
    void DeleteTexture(TextureLocation &texture);
};