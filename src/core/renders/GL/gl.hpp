#pragma once

#include <fstream>
#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "utils/logger.hpp"

#define GL_ZERO 0
#define GL_ZEROF 0.0f
#define GL_UNBIND 0

typedef unsigned int shader_program;
typedef unsigned int vertex_shader;
typedef unsigned int fragment_shader;
typedef unsigned int vertex_buff_loc;
typedef unsigned int vertex_attr_ptr;
typedef unsigned int element_buff_loc;
typedef unsigned int *element_buff_data;
typedef float *vertex_buff_data;
typedef unsigned int texture_loc;
typedef unsigned int uniform_var_loc;
typedef unsigned int frame_buff_loc;
typedef unsigned int depth_buff_loc;

namespace trimana_core
{
    struct gl_info
    {
        std::string gl_version{"Unknown"};
        std::string gl_vendor{"Unknown"};
        std::string gl_renderer{"Unknown"};
        std::string glsl_version{"#versio ?.?.?"};
        bool gl_load_success{false};
    };

    enum SHADER_TYPE : unsigned int
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

    enum COMPONENT_TYPE : int
    {
        COM_TY_XY = 0x2,
        COM_TY_XYZ = 0x3,
        COM_TY_RGB = 0x3,
        COM_TY_RGBA = 0x4,
        COM_TY_UV = 0x2
    };

    enum DATA_TYPE : unsigned int
    {
        FLOAT_TY = GL_FLOAT,
        UINT_TY = GL_UNSIGNED_INT,
        INT_TY = GL_INT
    };

    enum DRAW_TYPE : unsigned int
    {
        DRAW_STATIC = GL_STATIC_DRAW,
        DRAW_DYNAMIC = GL_DYNAMIC_DRAW
    };

    enum DRAW_CALLS : unsigned int
    {
        DRAW_POINTS = GL_POINTS,
        DRAW_TRIANGLES = GL_TRIANGLES,
        DRAW_LINES = GL_LINES
    };

    enum COLOR_CHANNELS : int
    {
        RGB = 0x3,
        RGBA = 0x4
    };

    class shader
    {
    public:
        shader() = default;
        ~shader() = default;


        shader_program shader_prog{GL_ZERO};
        vertex_shader vertex_shader_prog{GL_ZERO};
        fragment_shader fragment_shader_prog{GL_ZERO};

        std::string vertex_shader_code{"undefine"};
        std::string fragment_shader_code{"undefine"};

        bool shader_program_created{false};
    };

    class vertex_buffers
    {
    public:
        vertex_buffers() = default;
        ~vertex_buffers() = default;

        vertex_buff_loc vertex_buff{GL_ZERO};
        vertex_buff_loc color_buff{GL_ZERO};
        vertex_buff_loc texture_buff{GL_ZERO};
        vertex_buff_loc normals_buff{GL_ZERO};
        vertex_attr_ptr vertex_arry_ptr{GL_ZERO};
        element_buff_loc element_buff{GL_ZERO};
        unsigned int indices_count{GL_ZERO};
    };

    vertex_buffers *generate_buffers(unsigned int num_buffers);
    void assign_vertex_data(vertex_buffers *buffers, VERTEX_BUFFER_TYPE vtype, vertex_buff_data data, GLsizeiptr size, DRAW_TYPE dtype);
    void assign_element_data(vertex_buffers *buffers, element_buff_data data, GLsizeiptr size, DRAW_TYPE dtype);
    void link_vertex_buffers(vertex_buffers *buffers, shader_program &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
    void link_buffer_layout(unsigned int layout, vertex_buffers *buffers, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype);
    void link_element_buffers(vertex_buffers *buffer);
    void render(vertex_buffers *buffers, DRAW_CALLS dcall);
    void delete_buffers(vertex_buffers *buffers, unsigned int num_buffers_used);

    uniform_var_loc get_uniform_loc(shader_program &program, const std::string &uniform_val);
    shader *create_shader_program(const std::string &vshader, const std::string &fshader);
    void shader_attach(shader_program &program);
    void shader_dettach();
    bool delete_shaders_n_program(shader* programs);

    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, int data);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, unsigned int data);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, float data);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, float x, float y, float z, float w);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat4 &data);
    bool update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, glm::mat3 &data);

    texture_loc load_texture(const std::string &loc, COLOR_CHANNELS channels, bool flip = true);
    void texture_attach(texture_loc &texture);
    void texture_dettach();
    void delete_texture(texture_loc &texture);
};