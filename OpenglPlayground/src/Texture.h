#pragma once
#include <string>

enum class TextureType
{
	UNKNOW,
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	NORMAL,
};

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	std::string GetPath() const { return m_path; }
	TextureType m_type;
private:
	unsigned int m_rendererID;
	std::string m_path;
	int m_width, m_height, m_numColChannels;
	unsigned char* m_bufferData;
};
