#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "WindowManager.h"
#include "ParticleSystem.h"
#include "Solver.h"


int main() {
	WindowManager windowmanager;

	ParticleSystem system;
	system.SetBoundary(glm::vec2(-1.0f, -1.0f), glm::vec2(2.0f, 2.0f));
	system.AddFluidParticles(glm::vec2(-0.3f, -0.3f), glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, -1.0f), 0.01f * 0.7f);

	std::cout << "particle num: " << system.ParticleCount_ << std::endl;
	Solver fluidsolver(system);

	Shader shader("src/particle.vs", "src/particle.fs");

	Renderer renderer;

	unsigned int VAO, VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);



	while (!windowmanager.WindowShouldClose())
	{
		renderer.Clear();

		for (int i = 0; i < 10; i++) {
			system.SearchNeighbors();
			fluidsolver.Update();
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * system.ParticleCount_, system.Position_.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		shader.Bind();
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(5);
		
		glDrawArrays(GL_POINTS, 0, system.ParticleCount_);
		windowmanager.processInput();
		windowmanager.Update();
	}

	return 0;
}
