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
	Model(const std::string path, bool gamma = false);
	~Model();
	void DrawModel(Renderer& renderer);
private:
	void LoadModel();
	void ProcessNodes(const aiNode * node, const aiScene* scene);
	std::pair<std::vector<Vertex>,std::vector<unsigned int>>
	ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	
private:
	std::vector<Mesh> m_meshVec;
	std::vector<Texture> m_textureVec;
	std::string m_path;
	bool m_gammaCorrection;
};
