#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &RendererID_);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &RendererID_);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<VertexBufferElement> elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const VertexBufferElement element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.GetStride(), reinterpret_cast<void*>(offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(RendererID_);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
