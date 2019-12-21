#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> data, std::vector<unsigned> indices) :
	m_data(data), m_indices(indices)
{
	//setup the mesh for opengl
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(3);//pos
	m_VL->Push<float>(2);//tex
	m_VL->Push<float>(3);//nor

	m_VBO = new VertexBuffer(&m_data[0], m_data.size() * sizeof(Vertex));
	m_VAO->AddBuffer(*m_VBO, *m_VL);
	m_IBO = new IndexBuffer(&m_indices[0], m_indices.size());
	//unbind and wait for signal
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	
}


Mesh::~Mesh()
{
	//clean everything
	delete m_VBO;
	delete m_IBO;
	delete m_VL;
	delete m_VAO;
	m_data.clear();
	m_indices.clear();
}


void Mesh::Draw(Renderer& renderer)
{
	renderer.Draw(m_VAO, m_IBO);
}

std::vector<glm::vec3> Mesh::getPositions() const
{
	std::vector<glm::vec3> output;
	for (auto& p : m_data)
	{
		output.push_back(p.position);
	}
	return output;
}
