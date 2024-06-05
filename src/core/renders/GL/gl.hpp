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
    struct TRIMANA_CORE GLInfo
    {
        std::string GLVersion{"Unknown"};
        std::string GLVendor{"Unknown"};
        std::string GLRenderer{"Unknown"};
        std::string GLSLVersion{"#versio ?.?.?"};
        bool GL_LoadSuccess{false};
    };

    enum class SHADER_TYPE
    {
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
        COM_TY_XY = 0x2,
        COM_TY_XYZ = 0x3,
        COM_TY_RGB = 0x3,
        COM_TY_RGBA = 0x4,
        COM_TY_UV = 0x2
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

    class TRIMANA_CORE Shader
    {
    public:
        Shader() = default;
        ~Shader() = default;

        UniformVarLoc GetUniformLoc(ShaderProgramLoc &program, const std::string &uniform_val);
        Shader *CreateShaderProgram(const std::string &vshader, const std::string &fshader);
        void ShaderAttach(ShaderProgramLoc &program);
        void ShaderDettach();
        bool DeleteShaders_N_Programs(Shader *programs);

        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, int data);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, unsigned int data);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, float data);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, float x, float y);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, float x, float y, float z);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, glm::mat4 &data);
        bool UpdateUniformVariable(Shader SHADER_TYPE type, const std::string &uniform, glm::mat3 &data);

        std::string VertexShaderCode{"undefine"};
        std::string FragmentShaderCode{"undefine"};
        ShaderProgramLoc ShderProgramSelf{NULL};
        VertexShader VertexShaderProgram{NULL};
        FragmentShader FragmentShaderProgram{NULL};
        bool ShaderProgramCreated{false};
    };

    class TRIMANA_CORE VertexBuffers
    {
    public:
        VertexBuffers(unsigned int num_buffers);
        VertexBuffers(const VertexBuffers&) = delete;
        VertexBuffers& operator=(const VertexBuffers&) = delete;
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

    UniformVarLoc TRIMANA_CORE GetUniformLoc(ShaderProgramLoc &program, const std::string &uniform_val);
    Shader TRIMANA_CORE *CreateShaderProgram(const std::string &vshader, const std::string &fshader);
    void TRIMANA_CORE ShaderAttach(ShaderProgramLoc &program);
    void TRIMANA_CORE ShaderDettach();
    bool TRIMANA_CORE DeleteShaders_N_Programs(Shader *programs);

    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, int data);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, unsigned int data);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float data);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat4 &data);
    bool TRIMANA_CORE UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat3 &data);

    TextureLocation TRIMANA_CORE LoadTexture(const std::string &loc, COLOR_CHANNELS channels, bool flip = true);
    void TRIMANA_CORE TextureAttach(TextureLocation &texture);
    void TRIMANA_CORE TextureDettach();
    void TRIMANA_CORE DeleteTexture(TextureLocation &texture);
};