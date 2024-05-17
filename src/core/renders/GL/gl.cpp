#define GLM_ENABLE_EXPERIMENTAL
#include "gl.hpp"

trimana_core::vertex_buffers *trimana_core::generate_buffers(unsigned int num_buffers)
{
    vertex_buffers *buffers = new vertex_buffers();

    glGenBuffers(num_buffers, &buffers->vertex_arry_ptr);
    glGenBuffers(num_buffers, &buffers->vertex_buff);
    glGenBuffers(num_buffers, &buffers->texture_buff);
    glGenBuffers(num_buffers, &buffers->color_buff);
    glGenBuffers(num_buffers, &buffers->normals_buff);
    glGenBuffers(num_buffers, &buffers->element_buff);

    return buffers;
}

void trimana_core::assign_vertex_data(vertex_buffers *buffers, VERTEX_BUFFER_TYPE vtype, vertex_buff_data data, GLsizeiptr size, DRAW_TYPE dtype)
{
    if (buffers != nullptr)
    {
        glBindVertexArray(buffers->vertex_arry_ptr);

        switch (vtype)
        {
        case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->vertex_buff);
            break;

        case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->color_buff);
            break;

        case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->texture_buff);
            break;

        default:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->normals_buff);
            break;
        }

        glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
        glBindVertexArray(GL_UNBIND);
    }
}

void trimana_core::assign_element_data(vertex_buffers *buffers, element_buff_data data, GLsizeiptr size, DRAW_TYPE dtype)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->element_buff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, static_cast<GLenum>(dtype));
}

void trimana_core::link_vertex_buffers(vertex_buffers *buffers, shader_program &program, const std::string &attr, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
{
    uniform_var uniform_loc = glGetAttribLocation(program, attr.c_str());
    if (uniform_loc)
    {
        glBindVertexArray(buffers->vertex_arry_ptr);

        switch (vtype)
        {
       case VERTEX_BUFFER_TYPE::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->vertex_buff);
            break;

        case VERTEX_BUFFER_TYPE::COLOR_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->color_buff);
            break;

        case VERTEX_BUFFER_TYPE::TEXTURE_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->texture_buff);
            break;

        default:
            glBindBuffer(GL_ARRAY_BUFFER, buffers->normals_buff);
            break;
        }
    }
}
