#pragma once
#include<memory>
#include "../Singleton.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Mesh.h"

namespace Resources {
	class ResourcesManager : public Singleton<ResourcesManager> {
	public:
		std::string addShader(std::shared_ptr<ShaderProgram> &shader);

		std::shared_ptr<ShaderProgram> getShader(std::string shader_name);

	private:

		std::map<std::string, std::shared_ptr<ShaderProgram>> mShaderMap;
	
	};
}