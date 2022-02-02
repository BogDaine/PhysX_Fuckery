#include "IndexArray.h"
#include <iostream>

IndexArray::IndexArray()
{
	GLCall(glGenBuffers(1, &ID));
}
IndexArray::IndexArray(const void* data, const unsigned int& size, const unsigned int& drawingMode)
{
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawingMode));
	count = size / sizeof(unsigned int);
	this->size = size;
}

void IndexArray::setData(const void* data, const unsigned int& size, const unsigned int& drawingMode)
{
	
	bind();
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawingMode));
	count = size / sizeof(unsigned int);
	this->size = size;
}

IndexArray::~IndexArray()
{
	GLCall(glDeleteBuffers(1, &ID));
	std::cout << "INDEX ARRAY DESTROYED" << std::endl;
}

unsigned int IndexArray::getCount()
{
	return count;
}

void IndexArray::bind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void IndexArray::unbind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
