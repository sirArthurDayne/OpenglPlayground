#include "Texture.h"
#include "Renderer.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path) : m_rendererID(0), m_path(path),
				m_width(0), m_height(0), m_numColChannels(0), m_bufferData(nullptr)
{
	GLCALL(glGenTextures(1, &m_rendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	//render on area greater or smaller than texture size
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	//horizontal and vertical texture render
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));//default:GL_CLAMP
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	//always flip because of openGL texture coords system,(0,0) at bottom-left
	stbi_set_flip_vertically_on_load(1);
	
	m_bufferData = stbi_load(m_path.c_str(), &m_width, &m_height, &m_numColChannels, 4);
	
	if (m_bufferData)
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bufferData));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
		Unbind();
		stbi_image_free(m_bufferData);
		std::cout << "texture loaded! ->" << m_path <<"|| id:"<<m_rendererID << std::endl;
	}
	else std::cout << "FAILED TO LOAD TEXTURE, path:" << m_path;
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &m_rendererID));
}

void Texture::Bind(unsigned int slot  /*0*/ ) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0+slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}
	
void Texture::Unbind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}


