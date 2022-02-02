#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> tex)
{
	std::cout << "creating mesh!" << std::endl;
	vertices = vert;
	indices = ind;
	textures = tex;
	
	setupMesh();
}

void Mesh::Draw(Shader* shader)
{
	unsigned int diffuseNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	unsigned int specularNr = 0; // number of <type> textures

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i]->bind(i);

		std::string number;
		std::string name = textures[i]->getType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		shader->SetInt("material." + name + number, i);

		//std::cout << textures[i].getPath() << std::endl;
		//std::cout << std::endl;
	}
	//shader.bind();
	VAO.bind();
	IBO.bind();
	shader->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, IBO.getCount(), GL_UNSIGNED_INT, nullptr));

	VAO.unbind();
	shader->Unbind();
	IBO.unbind();
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i]->unbind();
	}
}

void Mesh::setupMesh()
{
	layout.push<float>(3); // COORDINATES
	layout.push<float>(3); // NORMALS
	layout.push<float>(2); // TEXTURE COORDINATES

	VBO.setData(vertices.data(), vertices.size() * sizeof(Vertex));
	IBO.setData(indices.data(), indices.size() * sizeof(unsigned int));
	//IBO = IndexArray(&indices[0], indices.size() * sizeof(unsigned int));
	//IBO.bind();

	//std::cout << " yesss" << std::endl;

	VAO.addBuffer(VBO, layout);
}
