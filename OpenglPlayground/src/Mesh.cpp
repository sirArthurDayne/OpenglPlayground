#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> data, std::vector<unsigned> indices) :
	m_data(data),m_indices(indices)
{
	//setup the mesh for opengl
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(3);//pos
	m_VL->Push<float>(2);//tex
	m_VL->Push<float>(3);//nor

	m_VBO = new VertexBuffer(&m_data[0], m_data.size() * sizeof(Vertex));
	m_VAO->AddBuffer(*m_VBO,*m_VL);
	m_IBO = new IndexBuffer(&m_indices[0], m_indices.size());
	//unbind and wait for signal
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	
}

Mesh::Mesh(std::vector<Vertex> data, std::vector<unsigned int> indices, std::vector<TextureData> textures) :
	m_data(data), m_indices(indices), m_texDataVec(textures)
{
		//setup the mesh for opengl
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(3);//pos
	m_VL->Push<float>(2);//tex
	m_VL->Push<float>(3);//nor

	m_VBO = new VertexBuffer(&m_data[0], m_data.size() * sizeof(Vertex));
	m_VAO->AddBuffer(*m_VBO,*m_VL);
	m_IBO = new IndexBuffer(&m_indices[0], m_indices.size());
	//unbind and wait for signal
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	LoadTextures();
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
	m_textures.clear();
}

void Mesh::LoadTextures()
{
	m_textures.reserve(4);
	for(auto& data : m_texDataVec)
		m_textures.emplace_back(data.path).m_type=data.type;
}


void Mesh::Draw(Renderer& renderer, Shader* shader)
{
	unsigned int i = 0;
	unsigned int diffNum = 1;
	unsigned int specNum = 1;
	for(auto& texture : m_textures)
	{
		std::string uniformName;
		if (texture.m_type == TextureType::DIFFUSE)
		{
			uniformName += "u_texture_diffuse_" + std::to_string(diffNum++);
		}
		else if (texture.m_type == TextureType::SPECULAR)
		{
			uniformName += "u_texture_specular_" + std::to_string(specNum++);
		}
		shader->SetUniform1i(uniformName, i);
		texture.Bind(i++);
	}
	renderer.Draw(m_VAO, m_IBO);
}