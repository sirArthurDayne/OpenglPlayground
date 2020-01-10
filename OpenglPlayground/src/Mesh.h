#pragma once
#include"glm.hpp"
#include "VertexLayout.h"
#include "Texture.h"
#include "Shader.h"

//TODO add tangents and bitangents for model normal mapping
struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
	glm::vec3 normal;
	Vertex():
		position(glm::vec3(0.0f)),
		textureCoord(glm::vec2(0.0f)),
		normal(glm::vec3(0.0f)) {}
	Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 norm):
		position(pos),
		textureCoord(tex),
		normal(norm) {}
	Vertex(glm::vec3 pos, glm::vec2 tex):
		position(pos),
		textureCoord(tex),
		normal(glm::vec3(0.0f)) {}
};
//TODO setup a material struct and test it.
struct Material
{
	glm::vec3 ambient;//ka
	glm::vec3 diffuse;//kd
	glm::vec3 specular;//ks
	float shininess;//sh
	Material() :
		ambient(glm::vec3(0.0f)),
		diffuse(glm::vec3(0.0f)),
		specular(glm::vec3(0.0f)),
		shininess(1.0f) {}
	Material(glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float sh) :
		ambient(amb),
		diffuse(dif),
		specular(spec),
		shininess(sh) {}
};


struct TextureData
{
	std::string path;
	TextureType type;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> data, std::vector<unsigned int> indices);
	Mesh(std::vector<Vertex> data, std::vector<unsigned int> indices, std::vector<TextureData> textures);
	void Draw(Renderer& renderer, Shader* shader);
	~Mesh();
private:
	void LoadTextures();
	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	VertexLayout* m_VL;
	IndexBuffer* m_IBO;
	std::vector<Vertex> m_data;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	std::vector<TextureData> m_texDataVec;
};
