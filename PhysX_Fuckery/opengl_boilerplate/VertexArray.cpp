#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &ID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteBuffers(1, &ID));
	std::cout << "VERTEX ARRAY DESTROYED" << std::endl;
}

void VertexArray::addBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto elements = layout.getElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}



void VertexArray::bind()
{
	GLCall(glBindVertexArray(ID));
}

void VertexArray::unbind()
{
	GLCall(glBindVertexArray(0));
}
