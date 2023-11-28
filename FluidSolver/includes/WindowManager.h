#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	GLFWwindow* window_;
	const unsigned int WindowHeight_ = 600;
	const unsigned int WindowWidth_ = 800;
	const char* WindowTitle_ = "FluidSim";
	int Initwindow();
	int WindowShouldClose();
	void processInput();
	void Update();

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* message, const void* userParamter);