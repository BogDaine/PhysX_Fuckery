#pragma once

#include "VertexStruct.h"
#include "Texture.h"
#include <vector>
#include <iostream>
#include "shader.h"
#include "opengl_boilerplate/VertexArray.h"
#include "opengl_boilerplate/VertexBuffer.h"
#include "opengl_boilerplate/VertexBufferLayout.h"
#include "opengl_boilerplate/IndexArray.h"
//#include <Renderer.h>

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	Mesh() {}
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> tex);
	void Draw(Shader* shader);
	//void Draw_by_Renderer(Shader& shader, Renderer& renderer);

	VertexArray VAO;
	VertexBuffer VBO;
	VertexBufferLayout layout;
	IndexArray IBO;

	void setupMesh();
	void sayHello() { std::cout << "HELLO" << std::endl; }
private:
};












//#pragma once
//
//#include <VertexStruct.h>
//#include "Texture.h"
//#include <vector>
//#include <iostream>
//#include "Shader.h"
//#include <VertexArray.h>
//#include <VertexBuffer.h>
//#include <VertexBufferLayout.h>
//#include <IndexArray.h>
//
//class Mesh
//{
//public:
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//	std::vector<Texture*> textures;
//
//	Mesh() {}
//	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> tex);
//	void Draw(Shader* shader);
//
//	VertexArray VAO;
//	VertexBuffer VBO;
//	VertexBufferLayout layout;
//	IndexArray IBO;
//
//	void setupMesh();
//	void sayHello() { std::cout << "HELLO" << std::endl; }
//private:
//};