#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "renders/GL/gl.hpp"
#include "utils/platform.hpp"
#include "utils/logger.hpp"

namespace TrimanaCore
{
    struct TRIMANA_CORE Vertexs
    {
        glm::vec3 Points;
        glm::vec2 UVs;
        glm::vec3 Normals;
        glm::vec4 ColorRGBA;
    };

    struct TRIMANA_CORE Mesh
    {
        Mesh() = default;
        ~Mesh() = default;

        std::vector<Vertexs> Vertices;
        std::vector<unsigned int> Indices;

        glm::mat4 MeshSelf{1.0f};

        unsigned int MeshIndex{NULL};
        unsigned int MaterialIndex{NULL};
        bool NoTextures{false};
    };

    struct TRIMANA_CORE Model
    {
        Model() = default;
        ~Model() = default;

        std::vector<std::shared_ptr<Mesh>> Meshes;
        std::vector<TextureLocation> Textures;

        unsigned int ModelIndex;
        bool ReadyToDraw{false};
    };

    class TRIMANA_CORE Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        void ImportModel(const std::string &model_file);
        void RenderModels();

    public:
        std::shared_ptr<Shader> ShaderPtr;
        std::shared_ptr<VertexBuffers> VtxBuffPtr;
        std::vector<std::shared_ptr<Model>> ModelsPtrs;
    };
}
