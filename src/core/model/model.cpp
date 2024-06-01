#include "model.hpp"

static void load_mesh(aiMesh *mesh, const aiScene *scene, TrimanaCore::model *loading_model)
{
    static unsigned int mesh_index = 0;
    TrimanaCore::mesh *mesh_data = new TrimanaCore::mesh();
    mesh_data->mesh_index = mesh_index++;

    // Importing vertices and normals
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 points(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        mesh_data->vertices[i].points = points;

        glm::vec3 normals(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        mesh_data->vertices[i].normals = normals;
    }

    // Color data (for now we go with a default color)
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 color(1.0f, 1.0f, 1.0f);
        glm::vec4 colorA(1.0f, 1.0f, 1.0f, 1.0f);

        mesh_data->vertices[i].colorRGB = color;
        mesh_data->vertices[i].colorRGBA = colorA;
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
                    mesh_data->vertices[j].uvs = tex_uv;
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
            mesh_data->vertices[j].uvs = tex_uv;
        }
    }

    // Indices data
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; i++)
        {
            mesh_data->indices.emplace_back(face.mIndices[i]);
        }
    }

    if(mesh->HasTextureCoords(0))
    {
        mesh_data->material_index = mesh->mMaterialIndex;
        mesh_data->no_texture = false;
    }
    else
    {
        mesh_data->material_index = NULL;
        mesh_data->no_texture = true;
    }

    loading_model->meshes.emplace_back(mesh_data);
}

static void load_node(aiNode* node, const aiScene* scene, TrimanaCore::model* loading_model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		load_mesh(scene->mMeshes[node->mMeshes[i]], scene, loading_model);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		load_node(node->mChildren[i], scene, loading_model);
	}
}

static void QGL_LoadMaterials(const aiScene* scene, TrimanaCore::model* loading_model)
{
	loading_model->textures.resize(scene->mNumMaterials);

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
					LOG_ERROR("Unable to load texture : {0} (loading with default textures)", texPath);
					texture = TrimanaCore::LoadTexture("Resources/Textures/plain.png", TrimanaCore::COLOR_CHANNELS::RGBA);
					loading_model->textures[materialIndex] = texture;
					break;
				}

				LOG_INFO("Texture Loading success : {0}", texPath);
				loading_model->textures[materialIndex] = texture;
			}
		}
	}
}
