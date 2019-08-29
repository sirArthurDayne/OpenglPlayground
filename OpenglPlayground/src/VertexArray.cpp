#include "VertexArray.h"
#include"Renderer.h"
#include"VertexLayout.h"
#include<iostream>

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_rendererID));
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexLayout& vl)
{
	Bind();
	vb.Bind();
	const auto& vbe = vl.GetVertexElements();
	unsigned int offsetPointer = 0;
	for (unsigned int i = 0; i < vbe.size(); i++)
	{
		const VertexBufferElement& element = vbe[i];
		//tell opengl how to work the specify buffer layout. ALWAYS HAS TO BE SET BEFORE DRAWING A BUFFER OBJECT 
		GLCALL(glEnableVertexAttribArray(i));
		//This function links the tri_buffer and his attributes (indices_buffer) with the current bound VAO
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, vl.GetStride(), (const void*)offsetPointer));
		offsetPointer += element.count * VertexBufferElement::GetTypeSize(element.type);
		std::cout << offsetPointer << "\n";
	}
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_rendererID));
}
