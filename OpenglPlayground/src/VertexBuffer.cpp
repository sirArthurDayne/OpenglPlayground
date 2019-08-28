#include"VertexBuffer.h"
#include"Renderer.h"
#include<iostream>
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//generates a buffer an assigns an id
	GLCALL(glGenBuffers(1, &m_rendererId));
	//binds the buffer to work with
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	//send the data to the buffer(size in bytes)
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	std::cout << "vbo created-->id:" << m_rendererId << std::endl;
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	std::cout << "vbo bind-->id:" << m_rendererId << std::endl;
}

void VertexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	std::cout << "vbo unbound-->id:" << m_rendererId << std::endl;
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_rendererId));
	std::cout << "vbo destroy-->id:" << m_rendererId << std::endl;
}
