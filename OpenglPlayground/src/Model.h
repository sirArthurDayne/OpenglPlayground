#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"
#include "assimp/Importer.hpp"
#include"assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#include <string>


class Model
{
public:
	Model(const std::string path);
	~Model();
	void DrawModel(Renderer& renderer, Shader* shader);
private:
	void LoadModel();
	void ProcessNodes(const aiNode * node, const aiScene* scene);
	std::tuple<std::vector<Vertex>,std::vector<unsigned int>,std::vector<TextureData>>
	ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<TextureData> LoadMaterialsTextures(aiMaterial* material, aiTextureType type);
		
private:
	std::vector<Mesh> m_meshLoaded;
	std::vector<TextureData> m_textureLoaded;
	std::string m_path, m_directory, m_fileName;
};
