#include "stdafx.h"
#include "ShaderProgram.h"
#include "ResourcesManager.h"

using namespace Resources;
std::string Resources::ResourcesManager::addShader(std::shared_ptr<ShaderProgram>& shader)
{
	std::string name = shader->GetFileName();
	if (mShaderMap.count(name) != 0)
	{
		mShaderMap.insert(std::make_pair(name, shader));
	}
	return name;
}

std::shared_ptr<ShaderProgram> Resources::ResourcesManager::getShader(std::string shader_name)
{
	//TODO Dokonczyc dodawanie w ShaderProgram
	return mShaderMap.at(shader_name);
}

