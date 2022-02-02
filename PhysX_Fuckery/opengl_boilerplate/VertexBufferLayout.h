#pragma once
#include <vector>
#include "GLCall.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int getSizeOfType(unsigned int typez)
	{
		switch (typez)
		{
		case GL_FLOAT:			return 4;//sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return 4;//sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return 1;//sizeof(GLbyte);
		}

		//ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0) {}

	//~VertexBufferLayout();

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)// , bool normalized)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		//m_Stride += sizeof(GLfloat);
		m_Stride += count * 4;
	}

	template<>
	void push<unsigned int>(unsigned int count)// , bool normalized)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += sizeof(GLuint) * count;
	}
	template<>
	void push<unsigned char>(unsigned int count)// , bool normalized)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += sizeof(GLbyte) * count;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }
};