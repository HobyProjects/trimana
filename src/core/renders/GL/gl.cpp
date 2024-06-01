#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "gl.hpp"

TrimanaCore::VertexBuffers *TrimanaCore::GenerateBuffers(unsigned int num_buffers)
{
    VertexBuffers *buffers = new VertexBuffers();

    glGenBuffers(num_buffers, &buffers->VertexArryPtr);
    glGenBuffers(num_buffers, &buffers->VertexBuff);
    glGenBuffers(num_buffers, &buffers->TextureBuff);
    glGenBuffers(num_buffers, &buffers->ColorBuff);
    glGenBuffers(num_buffers, &buffers->NormalsBuff);
    glGenBuffers(num_buffers, &buffers->element_buff);

    return buffers;
}

void TrimanaCore::AssignVertexBufferData(VertexBuffers *buffers, VERTEX_BUFFER_TYPE vtype, VertexBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
{
    if (buffers != nullptr)
    {
        glBindVertexArray(buffers->VertexArryPtr);

        switch (vtype)
        {
        case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->VertexBuff);
            break;

        case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->ColorBuff);
            break;

        case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->TextureBuff);
            break;

        default:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->NormalsBuff);
            break;
        }

        glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
        glBindVertexArray(GL_UNBIND);
    }
}

void TrimanaCore::AssignElementBufferData(VertexBuffers *buffers, ElementBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->element_buff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
}

void TrimanaCore::LinkVertexBuffers(VertexBuffers *buffers, ShaderProgram &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    UniformVarLoc uniform_loc = glGetAttribLocation(program, attr.c_str());
    if (uniform_loc)
    {
        glBindVertexArray(buffers->VertexArryPtr);

        switch (vtype)
        {
        case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->VertexBuff);
            break;

        case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->ColorBuff);
            break;

        case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->TextureBuff);
            break;

        default:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->NormalsBuff);
            break;
        }

        glVertexAttribPointer(uniform_loc, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, NULL, nullptr);
        glEnableVertexAttribArray(uniform_loc);
    }
}

void TrimanaCore::LinkBufferLayout(unsigned int layout, VertexBuffers *buffers, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    glBindVertexArray(buffers->VertexArryPtr);

    switch (vtype)
    {
    case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, buffers->VertexBuff);
        break;

    case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, buffers->ColorBuff);
        break;

    case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, buffers->TextureBuff);
        break;

    default:
        glBindBuffer(GL_ARRAY_BUFFER, buffers->NormalsBuff);
        break;
    }

    glVertexAttribPointer(layout, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, NULL, nullptr);
    glEnableVertexAttribArray(layout);
}

void TrimanaCore::LinkElementBuffers(VertexBuffers *buffers)
{
    if (buffers != nullptr)
    {
        glBindVertexArray(buffers->element_buff);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->element_buff);
        glBindVertexArray(GL_UNBIND);
    }
}

void TrimanaCore::Render(VertexBuffers *buffers, DRAW_CALLS dcall)
{
    glBindVertexArray(buffers->VertexArryPtr);

    if (buffers->element_buff != NULL)
    {
        glDrawElements(static_cast<GLenum>(dcall), buffers->IndicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(static_cast<GLenum>(dcall), NULL, buffers->IndicesCount);
    }

    glBindVertexArray(GL_UNBIND);
}

void TrimanaCore::DeleteBuffers(VertexBuffers *buffers, unsigned int num_buffers_used)
{
    glDeleteBuffers(num_buffers_used, &buffers->VertexBuff);
    glDeleteBuffers(num_buffers_used, &buffers->ColorBuff);
    glDeleteBuffers(num_buffers_used, &buffers->TextureBuff);
    glDeleteBuffers(num_buffers_used, &buffers->NormalsBuff);
    glDeleteVertexArrays(num_buffers_used, &buffers->VertexArryPtr);
    glDeleteBuffers(num_buffers_used, &buffers->element_buff);
    delete buffers;
}

UniformVarLoc TrimanaCore::GetUniformLoc(ShaderProgram &program, const std::string &uniform_val)
{
    if (program)
    {
        UniformVarLoc uniform = glGetUniformLocation(program, uniform_val.c_str());
        if (uniform)
        {
            return uniform;
        }

        LOG_ERROR("INVALID UNIFORM VARIABLE : {0}", uniform_val.c_str());
        return NULL;
    }

    LOG_ERROR("INVALID SHADER PROGRAM");
    return NULL;
}

static std::string import_shader(const std::string &shader_file)
{
    std::string fileContent{" "};
    std::fstream shaderFile(shader_file.c_str(), std::ios::in);

    if (!shaderFile.is_open())
    {
        LOG_CRITICAL("Unbale to open file: {0}", shader_file.c_str());
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

static ShaderProgram compile_shader_program(ShaderProgram &program, const std::string &shader_code, TrimanaCore::SHADER_TYPE shader_type)
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

        LOG_CRITICAL("SHADER PROGRAM COMPILE ERROR {0}", ((static_cast<GLenum>(shader_type) == GL_VERTEX_SHADER) ? "VERTEX-SHADER" : "FRAGMENT-SHADER"));
        LOG_WARN("SHADER PROGRAM CONTAINS FOLLOWING ERRORS : {0}", message);
        return NULL;
    }

    glAttachShader(program, shader);
    return shader;
}

TrimanaCore::Shader *TrimanaCore::CreateShaderProgram(const std::string &vshader, const std::string &fshader)
{
    Shader *shdr = new Shader();
    shdr->ShderProgramSelf = glCreateProgram();
    if (!shdr->ShderProgramSelf)
    {
        LOG_CRITICAL("SHADER PROGRAM WASN'T CREATED PROPERLY.");
        return nullptr;
    }

    shdr->VertexShaderCode = import_shader(vshader);
    shdr->FragmentShaderCode = import_shader(fshader);

    shdr->VertexShaderProgram = compile_shader_program(shdr->ShderProgramSelf, shdr->VertexShaderCode, SHADER_TYPE::VERTEX_SHADER);
    shdr->FragmentShaderProgram = compile_shader_program(shdr->ShderProgramSelf, shdr->FragmentShaderCode, SHADER_TYPE::FRAGMENT_SHADER);

    int result{0};
    int length{0};
    char *message{nullptr};

    glLinkProgram(shdr->ShderProgramSelf);
    glGetProgramiv(shdr->ShderProgramSelf, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        glGetProgramiv(shdr->ShderProgramSelf, GL_INFO_LOG_LENGTH, &length);
        message = (char *)alloca(length * sizeof(char));
        glGetProgramInfoLog(shdr->ShderProgramSelf, sizeof(message), &length, message);

        LOG_CRITICAL("SHADER PROGRAM LINKING ERROR -> {0}", message);
        return nullptr;
    }

    glValidateProgram(shdr->ShderProgramSelf);
    glGetProgramiv(shdr->ShderProgramSelf, GL_VALIDATE_STATUS, &result);

    if (result == GL_FALSE)
    {
        glGetProgramiv(shdr->ShderProgramSelf, GL_INFO_LOG_LENGTH, &length);
        message = (char *)alloca(length * sizeof(char));
        glGetProgramInfoLog(shdr->ShderProgramSelf, sizeof(message), &length, message);

        LOG_CRITICAL("SHADER PROGRAM VALIDATION ERROR -> {0}", message);
        return nullptr;
    }

    shdr->ShaderProgramCreated = true;
    return shdr;
}

void TrimanaCore::ShaderAttach(ShaderProgram &program)
{
    if (program)
    {
        glUseProgram(program);
    }
}

void TrimanaCore::ShaderDettach()
{
    glUseProgram(GL_UNBIND);
}

bool TrimanaCore::DeleteShaders_N_Programs(Shader *programs)
{
    if (programs != nullptr)
    {
        glDeleteShader(programs->VertexShaderProgram);
        glDeleteShader(programs->FragmentShaderProgram);
        glDeleteProgram(programs->ShderProgramSelf);
        delete programs;

        return true;
    }

    return false;
}

static UniformVarLoc uniform_update_validation(TrimanaCore::Shader *shdr, TrimanaCore::SHADER_TYPE type, const std::string &uniform_var)
{
    if (shdr != nullptr && shdr->ShderProgramSelf)
    {
        UniformVarLoc mem_loc{NULL};

        switch (type)
        {
        case TrimanaCore::SHADER_TYPE::VERTEX_SHADER:
            mem_loc = glGetUniformLocation(shdr->VertexShaderProgram, uniform_var.c_str());
            break;

        case TrimanaCore::SHADER_TYPE::FRAGMENT_SHADER:
            mem_loc = glGetUniformLocation(shdr->FragmentShaderProgram, uniform_var.c_str());
            break;

        default:
            break;
        }

        if (mem_loc)
            return mem_loc;
        else
            return NULL;
    }

    return NULL;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, int data)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform1i(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, unsigned int data)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform1ui(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float data)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform1f(location, data);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform2f(location, x, y);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform3f(location, x, y, z);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniform4f(location, x, y, z, w);
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat4 &data)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

    if (location)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
        return true;
    }

    return false;
}

bool TrimanaCore::UpdateUniformVariable(Shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat3 &data)
{
    UniformVarLoc location = uniform_update_validation(shdr, type, uniform);

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

    LOG_CRITICAL("UNABLE TO LOAD TEXTURE FROM {0}", loc.c_str());
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
