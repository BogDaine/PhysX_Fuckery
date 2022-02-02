#pragma once
#include "GLCall.h"

class VertexBuffer
{
private:
	unsigned int ID;
	void* data;
public:
	VertexBuffer();
	~VertexBuffer();
	VertexBuffer(void* data, unsigned int size, const unsigned int& drawingMode = GL_DYNAMIC_DRAW);
	void setData(void* data, unsigned int size, const unsigned int& drawingMode = GL_DYNAMIC_DRAW);
	void bind();
};