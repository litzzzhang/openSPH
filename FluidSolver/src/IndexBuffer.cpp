#include "IndexBuffer.h"
#include <glad/glad.h>


IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
	: count_(count)
{
	glGenBuffers(1, &RendererID_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &RendererID_);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID_);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

