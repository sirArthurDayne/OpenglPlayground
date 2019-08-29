#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
class Shader
{
public:
	Shader(const std::string& filepath);

	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& name, float* data);
private:
	std::pair<std::string, std::string> ParseShader(const std::string& filepath);
	void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int GetUniformLocation(const std::string& name);
public:
	~Shader();
private:
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_locationsCache;
	std::string m_filePath;
};

