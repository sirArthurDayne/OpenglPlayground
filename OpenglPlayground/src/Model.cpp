#include "Model.h"
#include <iostream>
#include "glm.hpp"

Model::Model(const std::string path, bool hasMaterials) :
	m_path(path), m_gammaCorrection(hasMaterials)
{
	m_directory = m_path.substr(0, m_path.find_last_of('/') + 1);
	int end = m_path.find_last_of('.')-1;
	int start = m_path.find_last_of('/');
	m_fileName = m_path.substr(m_path.find_last_of('/')+1, end - start);
	
	if (path!= "")
		LoadModel();
}

Model::~Model()
{
	m_meshLoaded.clear();
	m_textureLoaded.clear();
}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals| aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR model path:" << m_path <<"\n error-->" << importer.GetErrorString() << std::endl;
		importer.FreeScene();
		return;
	}
	ProcessNodes(scene->mRootNode, scene);
	importer.FreeScene();
}

void Model::ProcessNodes(const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto [vertices,indices,textures] = ProcessMesh(mesh, scene);
		m_meshLoaded.reserve(7);//TODO: elimnate this necesity to pre-allocate memory 
		m_meshLoaded.emplace_back(vertices, indices);
	}
	
	//recursily repeat process for child nodes
	for (unsigned int j = 0; j < node->mNumChildren; j++)
		ProcessNodes(node->mChildren[j], scene);
}

	std::tuple<std::vector<Vertex>,std::vector<unsigned int>,std::vector<Texture>>
	Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> out_vertices;
	std::vector<unsigned int> out_indices;
	std::vector<Texture> out_textures;

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

	//prepare materials/Textures data
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialsTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		out_textures.insert(out_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}	

	//make tuple
	return {out_vertices,out_indices,out_textures};
}

std::vector<Texture> Model::LoadMaterialsTextures(aiMaterial* material, aiTextureType type, std::string materialName)
{
	std::vector<Texture> out_vec;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString textureFileName;
		material->GetTexture(type, i, &textureFileName);
		
		bool exits = false;
		std::string fullpath = m_directory + textureFileName.C_Str();
		//compare for existing texture file
		for (auto& tex: m_textureLoaded)
		{
			if (tex.GetPath() == fullpath)
			{
				out_vec.push_back(tex);
				exits = true;
				break;
			}
		}
		//if the texture doesnt exits 
		if (!exits)
		{
			Texture t(fullpath);
			if (type == aiTextureType_DIFFUSE) t.m_type = TextureType::DIFFUSE;
			else if (type == aiTextureType_AMBIENT) t.m_type = TextureType::AMBIENT;
			else if (type == aiTextureType_SPECULAR) t.m_type = TextureType::SPECULAR;
			out_vec.push_back(t);
			m_textureLoaded.push_back(t);
		}
	}
	return out_vec;
}

void Model::DrawModel(Renderer& renderer)
{
	for (auto& mesh : m_meshLoaded)
		mesh.Draw(renderer);
}


