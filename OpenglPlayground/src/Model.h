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
	Model(const std::string path, bool hasMaterials = false);
	~Model();
	void DrawModel(Renderer& renderer);
private:
	void LoadModel();
	void ProcessNodes(const aiNode * node, const aiScene* scene);
	std::tuple<std::vector<Vertex>,std::vector<unsigned int>,std::vector<Texture>>
	ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialsTextures(aiMaterial* material, aiTextureType type, std::string materialName);
		
private:
	std::vector<Mesh> m_meshLoaded;
	std::vector<Texture> m_textureLoaded;
	std::string m_path, m_directory, m_fileName;
	bool m_gammaCorrection;
};
