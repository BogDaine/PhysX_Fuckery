#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	GLCall(glGenBuffers(1, &ID));

}
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &ID));
}
VertexBuffer::VertexBuffer(void* data, unsigned int size, const unsigned int& drawingMode)
{
	GLCall(glGenBuffers(1, &ID));
	bind();
	this->data = data;
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, drawingMode));
	//std::cout << std::endl << sizeof(data)  << std::endl;
}

void VertexBuffer::setData(void* data, unsigned int size, const unsigned int& drawingMode)
{
	bind();
	this->data = data;
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, drawingMode));
}

void VertexBuffer::bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
