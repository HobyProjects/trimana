#include "model.hpp"

static void LoadMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<TrimanaCore::Model> loading_model)
{
    static unsigned int mesh_index = 0;
    std::shared_ptr<TrimanaCore::Mesh> mesh_data = std::make_shared<TrimanaCore::Mesh>();
    mesh_data->MeshIndex = mesh_index++;

    // Importing vertices and normals
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 points(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        mesh_data->Vertices[i].Points = points;

        glm::vec3 normals(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        mesh_data->Vertices[i].Normals = normals;
    }

    // Color data (for now we go with a default color)
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec4 colorA(1.0f, 1.0f, 1.0f, 1.0f);
        mesh_data->Vertices[i].ColorRGBA = colorA;
    }

    // Texture coords
    if (mesh->HasTextureCoords(0))
    {
        for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
        {
            // We are double checking here becuse we don't want to loop until AI_MAX_NUMBER_OF_TEXTURECOORDS
            if (mesh->HasTextureCoords(i))
            {
                for (unsigned int j = 0; j < mesh->mNumVertices; j++)
                {
                    glm::vec2 tex_uv(mesh->mTextureCoords[i][j].x, mesh->mTextureCoords[i][j].y);
                    mesh_data->Vertices[j].UVs = tex_uv;
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            glm::vec2 tex_uv(0.0f, 0.0f);
            mesh_data->Vertices[j].UVs = tex_uv;
        }
    }

    // Indices data
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; i++)
        {
            mesh_data->Indices.emplace_back(face.mIndices[i]);
        }
    }

    if(mesh->HasTextureCoords(0))
    {
        mesh_data->MaterialIndex = mesh->mMaterialIndex;
        mesh_data->NoTextures = false;
    }
    else
    {
        mesh_data->MaterialIndex = NULL;
        mesh_data->NoTextures = true;
    }

    loading_model->Meshes.emplace_back(mesh_data);
}

static void LoadNode(aiNode* node, const aiScene* scene, std::shared_ptr<TrimanaCore::Model> loading_model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, loading_model);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, loading_model);
	}
}

static void LoadMaterials(const aiScene* scene, std::shared_ptr<TrimanaCore::Model> loading_model)
{
	loading_model->Textures.resize(scene->mNumMaterials);

	for (unsigned int materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
	{
		aiMaterial* material = scene->mMaterials[materialIndex];

		//TODO: for now we use aiTextureType_DIFFUSE only later we should implement for other texture types
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				size_t idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

                // [TODO]: enter the correct path
				std::string texPath = std::string("Resources/Textures/") + filename; 
				TextureLocation texture = TrimanaCore::LoadTexture(texPath, TrimanaCore::COLOR_CHANNELS::RGBA);

				if (!texture)
				{
					TRIMANA_CORE_ERROR("Unable to load texture : {0} (loading with default textures)", texPath);
					texture = TrimanaCore::LoadTexture("Resources/Textures/plain.png", TrimanaCore::COLOR_CHANNELS::RGBA);
					loading_model->Textures[materialIndex] = texture;
					break;
				}

				TRIMANA_CORE_INFO("Texture Loading success : {0}", texPath);
				loading_model->Textures[materialIndex] = texture;
			}
		}
	}
}

void TrimanaCore::Renderer::ImportModel(const std::string& model_file)
{
    TRIMANA_CORE_INFO("IMPORTING MODEL {0}", model_file.c_str());

    static unsigned int model_index = 0;
    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->ModelIndex = model_index++;

    Assimp::Importer model_importer;
    const aiScene* scene = model_importer.ReadFile(model_file, (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices));

    if(scene != nullptr)
    {
        LoadNode(scene->mRootNode, scene, model);
        LoadMaterials(scene, model);

        if(!model->Meshes.empty())
        {
            ModelsPtrs.emplace_back(model);
            return;
        }

        TRIMANA_CORE_ERROR("UNABLE TO IMPORT {0} MODEL, SOMETHING WENT WRONG WHILE IMPORTING..");
        return;
    }

    TRIMANA_CORE_ERROR("UNABLE TO IMPORT {0} MODEL, SOMETHING WENT WRONG WHILE IMPORTING..");
    TRIMANA_CORE_CRITICAL(model_importer.GetErrorString());
}

void TrimanaCore::Renderer::RenderModels()
{
    if(!ModelsPtrs.empty())
    {
        for(auto model : ModelsPtrs)
        {
            if(model->ReadyToDraw)
            {
                for(auto mesh : model->Meshes)
                {
                    if(mesh->NoTextures)
                    {
                        VtxBuffPtr->Render(DRAW_CALLS::DRAW_TRIANGLES);
                    }
                    else
                    {
                        TextureAttach(model->Textures[mesh->MaterialIndex]);
                        VtxBuffPtr->Render(DRAW_CALLS::DRAW_TRIANGLES);
                        TextureDettach();
                    }
                }
            }
        }
    }
}