#pragma once
#include"glm.hpp"
#include "VertexLayout.h"

//#include "assimp/Importer.hpp"
//#include"assimp/scene.h"
//#include"assimp/postprocess.h"

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

struct Material
{
	glm::vec3 ambient;//ka
	glm::vec3 diffuse;//kd
	glm::vec3 specular;//ks
	float shininess;//sh
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> data, std::vector<unsigned int> indices);
	void Draw(Renderer& renderer);
	~Mesh();
private:
	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	VertexLayout* m_VL;
	IndexBuffer* m_IBO;
	std::vector<Vertex> m_data;
	std::vector<unsigned int> m_indices;
};
