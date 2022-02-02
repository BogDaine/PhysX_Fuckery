#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <vector>

class Model
{
public:
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures_loaded;
	std::string directory;

	void LoadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	Model(std::string const& path)
	{
		LoadModel(path);
	}
	void Draw(Shader* shader);
};

//#include "Mesh.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <assimp/material.h>
//
//class Camera;
//
//class Model
//{
//protected:
//
//	Shader* m_BoundShader = nullptr;
//
//public:
//
//	Model() = default;
//
//	Model(std::string const& path)
//	{
//		LoadModel(path);
//	}
//
//	std::vector<Mesh*> meshes;
//
//	std::vector<Texture*> textures_loaded;
//	
//	std::string directory;
//
//	void SetShader(Shader*);
//	
//	Shader* GetShader();
//
//	void LoadModel(const std::string& path);
//
//	void processNode(aiNode* node, const aiScene* scene);
//
//	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
//
//	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
//
//
//
//	//as of now, the draw functions expects a shader with matrices ready to go
//
//	//void Draw(Camera*);
//
//	void Draw(Shader* shader);
//	void Draw();
//};