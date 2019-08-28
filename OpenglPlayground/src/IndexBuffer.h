#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* index_data, unsigned int count);
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_count; }
	~IndexBuffer();
private:
	unsigned int m_rendererId;
	unsigned int m_count;
};

