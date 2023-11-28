#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	GLboolean normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GL_FLOAT);
		case GL_UNSIGNED_INT:
			return sizeof(GL_UNSIGNED_INT);
		case GL_UNSIGNED_BYTE:
			return sizeof(GL_UNSIGNED_BYTE);
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : Stride_(0) {};

	void AddFloat(unsigned int count) { Push(GL_FLOAT, count, GL_FALSE); }
	void AddUnsignedInt(unsigned int count) { Push(GL_UNSIGNED_INT, count, GL_FALSE); }
	void AddUnsignedByte(unsigned int count) { Push(GL_UNSIGNED_BYTE, count, GL_FALSE); }

	inline const std::vector<VertexBufferElement> GetElements() const { return Elements_; }
	inline unsigned int GetStride() const { return Stride_; }

private:
	unsigned int Stride_;
	std::vector<VertexBufferElement> Elements_;
	
	void Push(unsigned int type, unsigned int count, GLboolean normalized) {
		Elements_.push_back({ type, count, normalized });
		Stride_ += count * VertexBufferElement::GetSizeOfType(type);
	}
};