#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "renders/GL/gl.hpp"

namespace trimana_core
{
    struct vertexs
    {
        glm::vec3 points;
        glm::vec2 uvs;
        glm::vec3 normals;
        glm::vec3 colorRGB;
        glm::vec4 colorRGBA;
    };

    struct mesh
    {
        mesh() = default;
        ~mesh() = default;

        std::vector<vertexs> vertices;
        std::vector<unsigned int> indices;
        
        glm::mat4 mesh_self{1.0f};

        unsigned int mesh_index{NULL};
        unsigned int material_index{NULL}; 
        bool no_texture{false};
    };

    struct model
    {
        std::vector<mesh*> meshes;
        std::vector<texture_loc> textures;
        vertex_buffers* buffers{nullptr};
        shader* shader_program{nullptr};
        bool ready_to_draw{false};
    };

    model* import_model(const std::string& model_file);
    void delete_model(model* model_ptr);
}
