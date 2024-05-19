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
    uniform_var_loc uniform_loc = glGetAttribLocation(program, attr.c_str());
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

        glVertexAttribPointer(uniform_loc, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
        glEnableVertexAttribArray(uniform_loc);
    }
}

void trimana_core::link_buffer_layout(unsigned int layout, vertex_buffers *buffers, VERTEX_BUFFER_TYPE vtype, COMPONENT_TYPE ctype)
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

    glVertexAttribPointer(layout, static_cast<GLint>(ctype), GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(layout);
}

void trimana_core::link_element_buffers(vertex_buffers* buffers)
{
    if(buffers != nullptr)
    {
        glBindVertexArray(buffers->element_buff);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->element_buff);
        glBindVertexArray(GL_UNBIND);
    }
}

void trimana_core::render(vertex_buffers *buffers, DRAW_CALLS dcall)
{
    glBindVertexArray(buffers->vertex_arry_ptr);

	if (buffers->element_buff != GL_ZERO)
	{
		glDrawElements(static_cast<GLenum>(dcall), buffers->indices_count, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(static_cast<GLenum>(dcall), GL_ZERO, buffers->indices_count);
	}

	glBindVertexArray(GL_UNBIND);
}

void trimana_core::delete_buffers(vertex_buffers* buffers, unsigned int num_buffers_used)
{
    glDeleteBuffers(num_buffers_used, &buffers->vertex_buff);
	glDeleteBuffers(num_buffers_used, &buffers->color_buff);
	glDeleteBuffers(num_buffers_used, &buffers->texture_buff);
	glDeleteBuffers(num_buffers_used, &buffers->normals_buff);
	glDeleteVertexArrays(num_buffers_used, &buffers->vertex_arry_ptr);
	glDeleteBuffers(num_buffers_used, &buffers->element_buff);
	delete buffers;
}

uniform_var_loc trimana_core::get_uniform_loc(shader_program &program, const std::string &uniform_val)
{
    if(program)
    {
        uniform_var_loc uniform = glGetUniformLocation(program, uniform_val.c_str());
        if(uniform)
        {
            return uniform;
        }

        LOG_ERROR("INVALID UNIFORM VARIABLE : {0}", uniform_val.c_str());
        return GL_ZERO;
    }

    LOG_ERROR("INVALID SHADER PROGRAM");
    return GL_ZERO;
}

static std::string import_shader(const std::string& shader_file)
{
	std::string fileContent{ " " };
	std::fstream shaderFile(shader_file.c_str(), std::ios::in);

	if (!shaderFile.is_open())
	{
		LOG_CRITICAL("Unbale to open file: {0}", shader_file.c_str());
		return fileContent;
	}
	else
	{
		std::string lines{ " " };
		while (!shaderFile.eof())
		{
			std::getline(shaderFile, lines);
			fileContent.append(lines + "\n");
		}

		shaderFile.close();
	}

	return fileContent;
}

static shader_program compile_shader_program(shader_program& program, const std::string& shader_code, trimana_core::SHADER_TYPE shader_type)
{
	unsigned int shader = glCreateShader(static_cast<GLenum>(shader_type));
	const char* src = shader_code.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);


	int result{ 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int mlength{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &mlength);
		char* message = (char*) alloca(mlength * sizeof(char));
		glGetShaderInfoLog(shader, sizeof(message), &mlength, message);

		LOG_CRITICAL("SHADER PROGRAM COMPILE ERROR {0}", ((static_cast<GLenum>(shader_type) == GL_VERTEX_SHADER) ? "VERTEX-SHADER" : "FRAGMENT-SHADER"));
		LOG_WARN("SHADER PROGRAM CONTAINS FOLLOWING ERRORS : {0}", message);
		return GL_ZERO;
	}

	glAttachShader(program, shader);
	return shader;
}

trimana_core::shader* trimana_core::create_shader_program(const std::string &vshader, const std::string &fshader)
{
    shader* shdr = new shader();
	shdr->shader_prog = glCreateProgram();
	if (!shdr->shader_prog)
	{
		LOG_CRITICAL("SHADER PROGRAM WASN'T CREATED PROPERLY.");
		return nullptr;
	}

	shdr->vertex_shader_code = import_shader(vshader);
	shdr->fragment_shader_code = import_shader(fshader);

	shdr->vertex_shader_prog = compile_shader_program(shdr->shader_prog, shdr->vertex_shader_code, SHADER_TYPE::VERTEX_SHADER);
	shdr->fragment_shader_prog = compile_shader_program(shdr->shader_prog, shdr->fragment_shader_code, SHADER_TYPE::FRAGMENT_SHADER);

	int result{ 0 };
	int length{ 0 };
	char* message{ nullptr };

	glLinkProgram(shdr->shader_prog);
	glGetProgramiv(shdr->shader_prog, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetProgramiv(shdr->shader_prog, GL_INFO_LOG_LENGTH, &length);
		message = (char*) alloca(length * sizeof(char));
		glGetProgramInfoLog(shdr->shader_prog, sizeof(message), &length, message);

		LOG_CRITICAL("SHADER PROGRAM LINKING ERROR -> {0}", message);
		return nullptr;
	}

	glValidateProgram(shdr->shader_prog);
	glGetProgramiv(shdr->shader_prog, GL_VALIDATE_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetProgramiv(shdr->shader_prog, GL_INFO_LOG_LENGTH, &length);
		message = (char*) alloca(length * sizeof(char));
		glGetProgramInfoLog(shdr->shader_prog, sizeof(message), &length, message);

		LOG_CRITICAL("SHADER PROGRAM VALIDATION ERROR -> {0}", message);
		return nullptr;
	}

    shdr->shader_program_created = true;
	return shdr;
}

void trimana_core::shader_attach(shader_program &program)
{
    if(program)
    {
        glUseProgram(program);
    }
}

void trimana_core::shader_dettach()
{
    glUseProgram(GL_UNBIND);
}

bool trimana_core::delete_shaders_n_program(shader* programs)
{
    if(programs != nullptr)
    {
        glDeleteShader(programs->vertex_shader_prog);
        glDeleteShader(programs->fragment_shader_prog);
        glDeleteProgram(programs->shader_prog);
        delete programs;

        return true;
    }

    return false;
}


static uniform_var_loc uniform_update_validation(trimana_core::shader* shdr, trimana_core::SHADER_TYPE type, const std::string& uniform_var)
{
    if(shdr != nullptr && shdr->shader_prog)
    {
        uniform_var_loc mem_loc{GL_ZERO};

        switch(type)
        {
            case trimana_core::SHADER_TYPE::VERTEX_SHADER:
                mem_loc = glGetUniformLocation(shdr->vertex_shader_prog, uniform_var.c_str());
                break;
            
            case trimana_core::SHADER_TYPE::FRAGMENT_SHADER:
                mem_loc = glGetUniformLocation(shdr->fragment_shader_prog, uniform_var.c_str());
                break;

            default:
                break;
        }

        if(mem_loc)
            return mem_loc;
        else
            return GL_ZERO;        
    }
}

bool trimana_core::update_uniform(shader *shdr, SHADER_TYPE type, const std::string &uniform, int data)
{
    uniform_var_loc location = uniform_update_validation(shdr, type, uniform);
    if(location)
    {
        glUniform1i(location, data);
        return true;
    }

    return false;
}


