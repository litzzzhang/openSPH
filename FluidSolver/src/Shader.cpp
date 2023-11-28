#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string VertexPath, std::string FragmentPath)
	:VertexPath_(VertexPath), FragmentPath_(FragmentPath), RendererID_(0)
{
	ShaderProgramSource source = ParseShader(VertexPath_, FragmentPath_);
	//std::cout << "VERTEX" << std::endl << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl << source.FragmentSource << std::endl;
	RendererID_ = CreateShader(source.VertexSource, source.FragmentSource);

	glUseProgram(RendererID_);
}

Shader::~Shader()
{
	std::cout << "Shader Delete" << std::endl;
	glDeleteProgram(RendererID_);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (UniformLocationCache_.find(name) != UniformLocationCache_.end())
		return UniformLocationCache_[name];

	int location = glGetUniformLocation(RendererID_, name.c_str());
	if (location == -1)
		std::cout << "No active uniform variable with given name " << name << " found" << std::endl;
	UniformLocationCache_[name] = location;
	return location;
}

struct ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::ifstream vertexStream(vertexPath);
	std::ifstream fragmentStream(fragmentPath);
	std::string line;
	std::stringstream ss[2];

	while (getline(vertexStream, line)) {
		ss[0] << line << '\n';
	}
	while (getline(fragmentStream, line)) {
		ss[1] << line << '\n';
	}

	struct ShaderProgramSource shadersources = { ss[0].str(), ss[1].str() };
	return shadersources;
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSource, const std::string fragmentShaderSource)
{
	
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint program_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
	std::cout << "Shader Program link status: " << (program_linked ? "success": "failed") << std::endl;
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cout << "Failed to link the shader program" << std::endl;
		std::cout << message << std::endl;
	}

	GLint program_valid;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &program_valid);
	std::cout << "Shader Program validate status: " << (program_valid ? "success" : "failed") << std::endl;
	if (program_valid != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cout << "Failed to validate the shader program" << std::endl;
		std::cout << message << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


unsigned int Shader::CompileShader(GLenum type, const std::string& source)
{
	unsigned int shaderID = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " 
		<< (result ? "success" : "failed") << std::endl;
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::cout
			<< "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader"
			<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

void Shader::Bind() const
{
	glUseProgram(RendererID_);

}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y)
{
	glUniform2f(GetUniformLocation(name), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

