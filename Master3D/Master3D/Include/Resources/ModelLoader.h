#pragma once

#include <Resources/Mesh.h>
#include <Resources/Material.h>

namespace Resources {
	class ModelLoader {
	public:
		ModelLoader();
		ModelLoader(const std::string &filename, bool smoothNormals, bool materialsEnabled = true);

		~ModelLoader();

		void LoadFile(const std::string &filename, bool smoothNormals);

		std::vector<Mesh>::const_iterator getMeshIterator();
		const std::vector<Mesh*>& getMeshList();
		const std::vector<Material*> getMaterialList();
		const bool isMaterialsEnabled();
	private:
		bool materialsEnabled;

		Logger * LOGGER = Logger::getLogger();

		void LoadScene(const aiScene * scene, bool smoothNormals);
		std::string mFilename;
		std::vector<Mesh*> mMeshesList;
		std::vector<Material*> mMateriaList;
		std::vector<Texture*> mTexList;

		void LoadTexture(const aiMaterial * material, aiTextureType texType, TextureType outTexType, Material * newMaterial, std::string texTypeName);
		bool isAbsolutePath(std::string path);
	};
}