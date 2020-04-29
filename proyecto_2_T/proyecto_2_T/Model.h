#pragma once
#include <vector>
#include<string>
#include<stdio.h>
#include<time.h>
#include<assimp/Importer.hpp>
#include<assimp/Exporter.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include <filesystem>
#include "Mesh.h"
#include "Texture.h"
namespace fs = std::experimental::filesystem;

class Model
{
public:
	Model();
	void LoadModel(const std::string& fileName);
	void LoadModel(const std::string & fileName, const std::string & fileNameOut);
	void RenderModel();
	void ClearModel();
	~Model();

private:
	void LoadNode(aiNode* node, const aiScene* scene); //assimp
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	const aiScene *scene;
	std::vector<Mesh*>MeshList;
	std::vector<Texture*>TextureList;
	std::vector<unsigned int>meshTotex;
};

