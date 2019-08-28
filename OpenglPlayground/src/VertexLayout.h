#pragma once
#include<vector>
#include<GL/glew.h>
#include"Renderer.h"

struct VertexBufferElement
{
public:
	unsigned int type;//data type
	unsigned int count;//Specifies the number of components per generic vertex attribute.
	unsigned char normalized;

	static unsigned int GetTypeSize(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:
		case GL_UNSIGNED_INT : return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};


class VertexLayout
{
public:
	VertexLayout() : m_stride(0) {};

	template<class T>
	void Push(unsigned int count) {
		static_assert(false);
	}
	template<>
	void Push<float>(unsigned int count){
		m_elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_stride += count * VertexBufferElement::GetTypeSize(GL_FLOAT);
	}
	template<>
	void Push<unsigned int>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE});
		m_stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_INT);
	}
	template<>
	void Push<unsigned char>(unsigned int count){
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_BYTE);
	}
	inline unsigned int GetStride() const { return m_stride; }
	inline const std::vector<VertexBufferElement>& GetVertexElements() const { return m_elements; }
	
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};

