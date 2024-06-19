#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gl.hpp"

TrimanaCore::VertexBuffers::VertexBuffers(unsigned int num_buffers)
{
    glGenBuffers(num_buffers, &VertexArryPtr);
    glGenBuffers(num_buffers, &VertexBuff);
    glGenBuffers(num_buffers, &TextureBuff);
    glGenBuffers(num_buffers, &ColorBuff);
    glGenBuffers(num_buffers, &NormalsBuff);
    glGenBuffers(num_buffers, &ElementBuff);

    NumOfBuffers = num_buffers;
}

TrimanaCore::VertexBuffers::~VertexBuffers()
{
    glDeleteBuffers(NumOfBuffers, &VertexBuff);
    glDeleteBuffers(NumOfBuffers, &ColorBuff);
    glDeleteBuffers(NumOfBuffers, &TextureBuff);
    glDeleteBuffers(NumOfBuffers, &NormalsBuff);
    glDeleteVertexArrays(NumOfBuffers, &VertexArryPtr);
    glDeleteBuffers(NumOfBuffers, &ElementBuff);
}

void TrimanaCore::VertexBuffers::AssignVertexBufferData(VERTEX_BUFFER_TYPE vtype, VertexBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
{
    glBindVertexArray(VertexArryPtr);

    switch (vtype)
    {
    case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuff);
        break;

    case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, ColorBuff);
        break;

    case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, TextureBuff);
        break;

    default:
        glBindBuffer(GL_ARRAY_BUFFER, NormalsBuff);
        break;
    }

    glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
    glBindVertexArray(GL_UNBIND);
}

void TrimanaCore::VertexBuffers::AssignElementBufferData(ElementBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
}

void TrimanaCore::VertexBuffers::LinkVertexBuffers(ShaderProgramLoc &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    UniformVarLoc uniform_loc = glGetAttribLocation(program, attr.c_str());
    if (uniform_loc)
    {
        glBindVertexArray(VertexArryPtr);

        switch (vtype)
        {
        case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, VertexBuff);
            break;

        case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, ColorBuff);
            break;

        case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, TextureBuff);
            break;

        default:
            glBindBuffer(GL_ARRAY_BUFFER, NormalsBuff);
            break;
        }

        glVertexAttribPointer(uniform_loc, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, NULL, nullptr);
        glEnableVertexAttribArray(uniform_loc);
    }
}

void TrimanaCore::VertexBuffers::LinkUsingBufferLayout(unsigned int layout, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    glBindVertexArray(VertexArryPtr);

    switch (vtype)
    {
    case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuff);
        break;

    case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, ColorBuff);
        break;

    case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, TextureBuff);
        break;

    default:
        glBindBuffer(GL_ARRAY_BUFFER, NormalsBuff);
        break;
    }

    glVertexAttribPointer(layout, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, NULL, nullptr);
    glEnableVertexAttribArray(layout);
}

void TrimanaCore::VertexBuffers::LinkElementBuffers()
{
    glBindVertexArray(ElementBuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuff);
    glBindVertexArray(GL_UNBIND);
}

void TrimanaCore::VertexBuffers::Render(DRAW_CALLS dcall)
{
    glBindVertexArray(VertexArryPtr);

    if (ElementBuff != NULL)
    {
        glDrawElements(static_cast<GLenum>(dcall), IndicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(static_cast<GLenum>(dcall), NULL, IndicesCount);
    }

    glBindVertexArray(GL_UNBIND);
}

TrimanaCore::Shader::Shader(const std::string &vshader, const std::string &fshader)
{
    ShderProgramSelf = glCreateProgram();
    if (!ShderProgramSelf)
    {
        TRIMANA_CORE_CRITICAL("SHADER PROGRAM WASN'T CREATED PROPERLY.");
        return;
    }

    VertexShaderCode = ImportShader(vshader);
    FragmentShaderCode = ImportShader(fshader);

    VertexShaderProgram = CompileShaderProgram(ShderProgramSelf, VertexShaderCode, SHADER_TYPE::VERTEX_SHADER);
    FragmentShaderProgram = CompileShaderProgram(ShderProgramSelf, FragmentShaderCode, SHADER_TYPE::FRAGMENT_SHADER);

    int result{0};
    int length{0};
    char *message{nullptr};

    glLinkProgram(ShderProgramSelf);
    glGetProgramiv(ShderProgramSelf, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        glGetProgramiv(ShderProgramSelf, GL_INFO_LOG_LENGTH, &length);
        message = (char *)alloca(length * sizeof(char));
        glGetProgramInfoLog(ShderProgramSelf, sizeof(message), &length, message);

        TRIMANA_CORE_CRITICAL("SHADER PROGRAM LINKING ERROR -> {0}", message);
        return;
    }

    glValidateProgram(ShderProgramSelf);
    glGetProgramiv(ShderProgramSelf, GL_VALIDATE_STATUS, &result);

    if (result == GL_FALSE)
    {
        glGetProgramiv(ShderProgramSelf, GL_INFO_LOG_LENGTH, &length);
        message = (char *)alloca(length * sizeof(char));
        glGetProgramInfoLog(ShderProgramSelf, sizeof(message), &length, message);

        TRIMANA_CORE_CRITICAL("SHADER PROGRAM VALIDATION ERROR -> {0}", message);
        return;
    }

    ShaderProgramCreated = true;
}

TrimanaCore::Shader::~Shader()
{
    glDeleteShader(VertexShaderProgram);
    glDeleteShader(FragmentShaderProgram);
    glDeleteProgram(ShderProgramSelf);
}

UniformVarLoc TrimanaCore::Shader::GetUniformLoc(SHADER_TYPE program, const std::string &uniform_val)
{
    UniformVarLoc uniform{NULL};

    switch (program)
    {
    case SHADER_TYPE::VERTEX_SHADER:
        uniform = glGetUniformLocation(VertexShaderProgram, uniform_val.c_str());
        break;

    case SHADER_TYPE::FRAGMENT_SHADER:
        uniform = glGetUniformLocation(FragmentShaderProgram, uniform_val.c_str());
        break;

    case SHADER_TYPE::SHADER_PROGRAM:
        uniform = glGetUniformLocation(ShderProgramSelf, uniform_val.c_str());
        break;

    default:
        break;
    }

    if (uniform)
    {
        return uniform;
    }

    TRIMANA_CORE_ERROR("INVALID SHADER PROGRAM OR INVALID UNIFORM VARIABLE");
    return NULL;
}

std::string TrimanaCore::Shader::ImportShader(const std::string &shader_file)
{
    std::string fileContent{" "};
    std::fstream shaderFile(shader_file.c_str(), std::ios::in);

    if (!shaderFile.is_open())
    {
        TRIMANA_CORE_CRITICAL("Unbale to open file: {0}", shader_file.c_str());
        return fileContent;
    }
    else
    {
        std::string lines{" "};
        while (!shaderFile.eof())
        {
            std::getline(shaderFile, lines);
            fileContent.append(lines + "\n");
        }

        shaderFile.close();
    }

    return fileContent;
}

ShaderProgramLoc TrimanaCore::Shader::CompileShaderProgram(ShaderProgramLoc &program, const std::string &shader_code, TrimanaCore::SHADER_TYPE shader_type)
{
    unsigned int shader = glCreateShader(static_cast<GLenum>(shader_type));
    const char *src = shader_code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int result{0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int mlength{0};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &mlength);
        char *message = (char *)alloca(mlength * sizeof(char));
        glGetShaderInfoLog(shader, sizeof(message), &mlength, message);

        TRIMANA_CORE_CRITICAL("SHADER PROGRAM COMPILE ERROR {0}", ((static_cast<GLenum>(shader_type) == GL_VERTEX_SHADER) ? "VERTEX-SHADER" : "FRAGMENT-SHADER"));
        TRIMANA_CORE_WARN("SHADER PROGRAM CONTAINS FOLLOWING ERRORS : {0}", message);
        return NULL;
    }

    glAttachShader(program, shader);
    return shader;
}

void TrimanaCore::Shader::ShaderAttach()
{
    if (ShderProgramSelf)
    {
        glUseProgram(ShderProgramSelf);
    }
}

void TrimanaCore::Shader::ShaderDettach()
{
    glUseProgram(GL_UNBIND);
}

UniformVarLoc TrimanaCore::Shader::UniformUpdateValidation(SHADER_TYPE type, const std::string &uniform_var)
{
    UniformVarLoc mem_loc{NULL};

    switch (type)
    {
    case SHADER_TYPE::VERTEX_SHADER:
        mem_loc = glGetUniformLocation(VertexShaderProgram, uniform_var.c_str());
        break;

    case SHADER_TYPE::FRAGMENT_SHADER:
        mem_loc = glGetUniformLocation(FragmentShaderProgram, uniform_var.c_str());
        break;

    case SHADER_TYPE::SHADER_PROGRAM:
        mem_loc = glGetUniformLocation(FragmentShaderProgram, uniform_var.c_str());
        break;

    default:
        break;
    }

    if (mem_loc)
        return mem_loc;
    else
        return NULL;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, int data)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform1i(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, unsigned int data)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform1ui(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float data)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform1f(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform2f(location, x, y);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y, float z)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform3f(location, x, y, z);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniform4f(location, x, y, z, w);
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, glm::mat4 &data)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
        return true;
    }

    return false;
}

bool TrimanaCore::Shader::UpdateUniformVariable(SHADER_TYPE type, const std::string &uniform, glm::mat3 &data)
{
    UniformVarLoc location = UniformUpdateValidation(type, uniform);

    if (location)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
        return true;
    }

    return false;
}

TextureLocation TrimanaCore::LoadTexture(const std::string &loc, COLOR_CHANNELS channels, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    int width{NULL}, height{NULL}, BPP{NULL};
    unsigned char *texture_buffer = stbi_load(loc.c_str(), &width, &height, &BPP, static_cast<int>(channels));

    if (texture_buffer != nullptr)
    {
        TextureLocation ID{NULL};
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, NULL, (channels == COLOR_CHANNELS::RGB) ? GL_RGB8 : GL_RGBA8, width, height, 0, (channels == COLOR_CHANNELS::RGBA) ? GL_RGB8 : GL_RGBA8, GL_UNSIGNED_BYTE, texture_buffer);

        glBindTexture(GL_TEXTURE_2D, NULL);

        return ID;
    }

    TRIMANA_CORE_CRITICAL("UNABLE TO LOAD TEXTURE FROM {0}", loc.c_str());
    return NULL;
}

void TrimanaCore::TextureAttach(TextureLocation &texture)
{
    glBindTexture(GL_TEXTURE_3D, texture);
}

void TrimanaCore::TextureDettach()
{
    glBindTexture(GL_TEXTURE_3D, GL_UNBIND);
}

void TrimanaCore::DeleteTexture(TextureLocation &texture)
{
    if (texture)
    {
        glDeleteTextures(1, &texture);
    }
}
