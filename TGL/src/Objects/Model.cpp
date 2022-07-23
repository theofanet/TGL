#include "tglpch.h"
#include "Model.h"
#include "Core/Registry.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


Model::Model(const std::string& filePath) {
	LoadModel(filePath);
}

Model::~Model() {

}

void Model::LoadModel(const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	ASSERT(scene && scene->mRootNode, "ERROR::ASSIMP::{}", importer.GetErrorString());

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene){
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene);
}

Ref<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene){
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	Ref<Material> material = CreateRef<Material>();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		if (mesh->mTextureCoords[0]) {
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(.0f);
			vertices.push_back(.0f);
		}
		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0){
		aiMaterial* AiMat = scene->mMaterials[mesh->mMaterialIndex];

		material->SetSpecular({ 0.5f, .5f, .5f });
		material->SetShininess(.5f);

		Ref<Texture> amb = LoadMaterialTextures(AiMat, aiTextureType_DIFFUSE, "texture_diffuse");
		Ref<Texture> spec = LoadMaterialTextures(AiMat, aiTextureType_SPECULAR, "texture_specular");
		if(amb)
			material->SetAmbientMap(amb);
		if (spec)
			material->SetSpecularMap(spec);
	}

	return CreateRef<Mesh>(vertices, indices, material);
}

Ref<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++){
		aiString str;
		mat->GetTexture(type, i, &str);
		INFO("{} : {}", typeName, str.C_Str());
		return Registry::GetTexture(str.C_Str());
	}
	return nullptr;
}