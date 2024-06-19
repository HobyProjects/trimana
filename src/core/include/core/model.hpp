#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "gl.hpp"
#include "logger.hpp"

namespace TrimanaCore
{
    struct Vertexs
    {
        glm::vec3 Points;
        glm::vec2 UVs;
        glm::vec3 Normals;
        glm::vec4 ColorRGBA;
    };

    struct Mesh
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

    struct Model
    {
        Model() = default;
        ~Model() = default;

        std::vector<std::shared_ptr<Mesh>> Meshes;
        std::vector<TextureLocation> Textures;

        unsigned int ModelIndex;
        bool ReadyToDraw{false};
    };

    class Renderer
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
