#pragma once


#include "Mesh.h"
#include <assimp/scene.h>
#include "Graphics/Material.h"

class Model {
public:
	Model(const std::string& filePath);
	~Model();

	void LoadModel(const std::string& filePath);

	inline const std::vector<Ref<Mesh>>& GetMeshes() { return m_Meshes; }

protected:
	Ref<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Ref<Mesh>> m_Meshes;
	Ref<Material> m_Material;
};