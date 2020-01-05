#include "Model.h"
#include <iostream>
#include "glm.hpp"

Model::Model(const std::string path, bool gamma) :
	m_path(path), m_gammaCorrection(gamma)
{
	if (path!= "")
		LoadModel();
}

Model::~Model()
{
	m_meshVec.clear();
	m_textureVec.clear();
}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals| aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR model path:" << m_path <<"\n error-->" << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessNodes(scene->mRootNode, scene);
}

void Model::ProcessNodes(const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto [vertices, indices] = ProcessMesh(mesh, scene);
		m_meshVec.emplace_back(vertices, indices);
	}
	
	//recursily repeat process for child nodes
	for (unsigned int j = 0; j < node->mNumChildren; j++)
		ProcessNodes(node->mChildren[j], scene);
}

	std::pair<std::vector<Vertex>,std::vector<unsigned int>>
	Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> out_vertices;
	std::vector<unsigned int> out_indices;

	//vertex data
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertex.position = v;
		v.x = mesh->mNormals[i].x;
		v.y = mesh->mNormals[i].y;
		v.z = mesh->mNormals[i].z;
		vertex.normal = v;
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 v;
			v.x = mesh->mTextureCoords[0][i].x;
			v.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoord = v;
		}
		else vertex.textureCoord = glm::vec2(0.0f);
		out_vertices.push_back(vertex);
	}
	
	//indices data
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				out_indices.push_back(face.mIndices[j]);
		}
	//make pair
	return {out_vertices, out_indices};
}

void Model::DrawModel(Renderer& renderer)
{
	for (auto& mesh : m_meshVec)
		mesh.Draw(renderer);
}


