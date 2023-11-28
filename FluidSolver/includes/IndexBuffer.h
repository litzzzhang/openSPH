#pragma once

class IndexBuffer {
private:
	unsigned int RendererID_;
	unsigned int count_;

public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count_; }
};