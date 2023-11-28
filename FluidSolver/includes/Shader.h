#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader {
private:

	unsigned int RendererID_;
	std::string VertexPath_;
	std::string FragmentPath_;
	std::unordered_map<std::string, int> UniformLocationCache_;

	int GetUniformLocation(const std::string& name);
	struct ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	unsigned int CreateShader(const std::string& vertexShaderSource, const std::string fragmentShaderSource);
	unsigned int CompileShader(GLenum type, const std::string& source);

public:
	Shader(std::string VertexPath, std::string FragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetVec2(const std::string& name, float x, float y);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec3(const std::string& name, float x, float y, float z);
	void SetVec4(const std::string& name, const glm::vec4& value);
	void SetVec4(const std::string& name, float x, float y, float z, float w);
	void SetMat2(const std::string& name, const glm::mat2& mat);
	void SetMat3(const std::string& name, const glm::mat3& mat);
	void SetMat4(const std::string& name, const glm::mat4& mat);

};