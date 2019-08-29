#include"IndexBuffer.h"
#include"Renderer.h"
#include<iostream>

IndexBuffer::IndexBuffer(const unsigned int* index_data, unsigned int count): m_count(count)
{
	GLCALL(glGenBuffers(1, &m_rendererId));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	//count (the total amount of indices)
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), index_data, GL_STATIC_DRAW));
	std::cout << "ibo created-->id: " << m_rendererId<< std::endl;
}
 
void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	std::cout << "ibo bound-->id: " << m_rendererId<< std::endl;
}

void IndexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	std::cout << "ibo Unbound-->id: " << m_rendererId<< std::endl;
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_rendererId));
	std::cout << "ibo destroy-->id: " << m_rendererId<< std::endl;
}
