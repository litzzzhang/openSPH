#pragma once

#include "Renderer.h"

void Renderer::Clear() const
{
	glClearColor(0.77f,0.77f,0.77f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawTriangle(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawPoint(const VertexArray& va, const Shader& shader, const int pointNum) const
{
	shader.Bind();
	va.Bind();
	glDrawArrays(GL_POINTS, 0, pointNum);
}
