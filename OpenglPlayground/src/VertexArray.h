#pragma once
#include"VertexBuffer.h"
#include"VertexLayout.h"


class VertexArray
{
public:
	VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddBuffer(const VertexBuffer& vb, const VertexLayout& vl);
	~VertexArray();
private:
	unsigned int m_rendererID;
};

