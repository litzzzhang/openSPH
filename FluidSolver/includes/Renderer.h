#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void Clear() const;
	void DrawTriangle(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawPoint(const VertexArray& va, const Shader& shader, const int pointNum) const;
};