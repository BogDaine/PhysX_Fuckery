#pragma once
#include "GLCall.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:
	unsigned int ID;
public:
	VertexArray();
	~VertexArray();
	void addBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind();
	static void unbind();
};