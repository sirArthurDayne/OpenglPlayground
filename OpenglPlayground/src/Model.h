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
	void DrawModel(Renderer& renderer, Shader* shader);
	
	void UpdateModel(const glm::vec3 traslation, const glm::vec3 scale, const glm::vec3 direction, const float angleRadians);
	glm::mat4 GetModelMatrix(void) const { return m_modelMatrix; }
private:
	void LoadModel();
	void ProcessNodes(const aiNode * node, const aiScene* scene);
	std::tuple<std::vector<Vertex>,std::vector<unsigned int>,std::vector<TextureData>, std::vector<Material>>
	ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<TextureData> LoadMaterialsTextures(aiMaterial* material, aiTextureType type);
		
private:
	std::vector<Mesh> m_meshLoaded;
	std::vector<TextureData> m_textureLoaded;
	std::vector<Material> m_materialsLoaded;
	std::string m_path, m_directory, m_fileName;
	bool m_hasMaterials = false;

	glm::mat4 m_modelMatrix;
};
