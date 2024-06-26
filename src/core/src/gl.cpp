#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gl.hpp"

std::string TrimanaCore::GL::mGLVersion = "UNKNOWN";
std::string TrimanaCore::GL::mGLVendor = "UNKNOWN";
std::string TrimanaCore::GL::mGLRenderer = "UNKNOWN";
std::string TrimanaCore::GL::mGLSLVersion = "UNKNOWN";
bool TrimanaCore::GL::GL_LoadSuccess = false;

bool TrimanaCore::GL::Load()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        TRIMANA_CORE_CRITICAL("UNABLE TO LOAD GL FUNCTIONS");
        GL_LoadSuccess = false;
        return GL_LoadSuccess;
    }

    glEnable(GL_DEPTH_TEST);
    mGLVersion = std::string((const char *)glGetString(GL_VERSION));
    mGLRenderer = std::string((const char *)glGetString(GL_RENDERER));
    mGLVendor = std::string((const char *)glGetString(GL_VENDOR));
    mGLSLVersion = std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

    char GL_MINOR = mGLVersion[2];
    char GL_PATCH = mGLVersion[4];
    char GL_MAJOR = mGLVersion[0];

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR);

    std::stringstream ss;
    ss << "#version " << GL_MAJOR << GL_MINOR << GL_PATCH << " core";
    mGLSLVersion = ss.str();
    GL_LoadSuccess = true;
    return GL_LoadSuccess;
}

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

void TrimanaCore::VertexBuffers::BufferData(VERTEX_BUFFER_TYPE vtype, VertexBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
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

    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(dtype));
    glBindVertexArray(GL_UNBIND);
}

void TrimanaCore::VertexBuffers::BufferData(ElementBufferData data, GLsizeiptr size, DRAW_TYPE dtype)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
}

void TrimanaCore::VertexBuffers::LinkBuffers(ShaderProgramLoc &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    UniformVarLoc uniform_loc = glGetAttribLocation(program, attr.c_str());
    if (uniform_loc)
    {
        glBindVertexArray(VertexArryPtr);
        glEnableVertexAttribArray(uniform_loc);

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
    }
}

void TrimanaCore::VertexBuffers::LinkUsingLayout(unsigned int layout, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    glBindVertexArray(VertexArryPtr);
    glEnableVertexAttribArray(layout);

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
}

void TrimanaCore::VertexBuffers::LinkElementBuffers()
{
    glBindVertexArray(ElementBuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuff);
}

void TrimanaCore::VertexBuffers::Render(DRAW_CALLS dcall, bool draw_elements)
{
    glBindVertexArray(VertexArryPtr);

    if (draw_elements)
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
    std::fstream shaderFile;

    shaderFile.open(shader_file.c_str(), std::ios::in);
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

void TrimanaCore::Shader::Attach()
{
    if (ShderProgramSelf)
    {
        glUseProgram(ShderProgramSelf);
    }
}

void TrimanaCore::Shader::Dettach()
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

static float RectVertices[] =
    {
        // Coords
        1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f, 1.0f,

        1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f};

static float RectTexUV[] =
    {
        // texCoords
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f};

TrimanaCore::FrameBuffer::FrameBuffer(glm::vec2 sizes)
{
    GLsizei width = (GLsizei)sizes.x;
    GLsizei height = (GLsizei)sizes.y;

    glGenFramebuffers(GL_DEFAULT_GENERATE_BUFFERS, &mFrameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);

    glGenTextures(GL_DEFAULT_GENERATE_BUFFERS, &mFrameTexture);
    glBindTexture(GL_TEXTURE_2D, mFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, width, height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, NULL);
    glBindTexture(GL_TEXTURE_2D, GL_UNBIND);

    // glGenTextures(GL_DEFAULT_GENERATE_BUFFERS, &mFrameTextureDepth);
    // glBindTexture(GL_TEXTURE_2D, mFrameTextureDepth);
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mFrameTextureDepth, NULL);
    // glBindTexture(GL_TEXTURE_2D, NULL);

    glGenRenderbuffers(GL_DEFAULT_GENERATE_BUFFERS, &mRenderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, GL_UNBIND);

    auto framebuff_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebuff_status != GL_FRAMEBUFFER_COMPLETE)
    {
        TRIMANA_CORE_CRITICAL("FRAME BUFFER ERROR : {0}", framebuff_status);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, GL_UNBIND);
}

TrimanaCore::FrameBuffer::~FrameBuffer()
{
    glDeleteTextures(1, &mFrameTexture);
    //glDeleteTextures(1, &mFrameTextureDepth);
    
    glDeleteFramebuffers(GL_FRAMEBUFFER, &mFrameBufferObject);
    glDeleteRenderbuffers(GL_RENDERBUFFER, &mRenderBufferObject);
}

void TrimanaCore::FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
    glEnable(GL_DEPTH_TEST);
}

void TrimanaCore::FrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GL_UNBIND);
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    // glDrawBuffers(mFrameTexture, buffers);
}

void TrimanaCore::FrameBuffer::FrameResize(glm::vec2 sizes)
{
    GLsizei width = (GLsizei)sizes.x;
    GLsizei height = (GLsizei)sizes.y;

    glBindTexture(GL_TEXTURE_2D, mFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, width, height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_UNBIND);

    // glBindTexture(GL_TEXTURE_2D, mFrameTextureDepth);
    // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mFrameTextureDepth, NULL);
    // glBindTexture(GL_TEXTURE_2D, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, GL_UNBIND);
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
