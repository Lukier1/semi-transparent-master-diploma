#include "stdafx.h"

#include <Resources\ShaderProgram.h>
#include <ShaderManager.h>
#include <Exceptions\MissingResourceException.h>



const Resources::ShaderProgram * ShaderManager::getShader(std::string name)
{
	if (shadersMap.count(name) == 0) {
		throw new MissingResourceException("Shader Pointer", "Name: " + name);
	}

	return shadersMap[name].get();
}

void ShaderManager::putShader(std::string name, Resources::ShaderProgramPtr shaderPtr)
{
	if (shadersMap.count(name) == 0) {
		shadersMap.insert(std::make_pair<>(name, shaderPtr));
		//TODO za ma³o dla klucza

		for (int i = 0; i < Resources::SHADER_TYPE_NUM; ++i) {
			ShaderNameType shaderNameType;
			shaderNameType.filename = shaderPtr->GetShadersFileName((Resources::SHADER_TYPE)i);
			shaderNameType.type = (Resources::SHADER_TYPE)i;
			shaderNameType.hash = hashForFile(shaderNameType.filename);
			shaderNameType.name = name;
			mapShaderfiles.insert(std::make_pair <>(name + std::to_string(i), shaderNameType));
		}
	}
}

void ShaderManager::scan(float delta)
{
	for (auto iterator = mapShaderfiles.begin(); iterator != mapShaderfiles.end(); ++iterator) {
		ShaderNameType shaderNameType = iterator->second;
		std::string newHash = hashForFile(shaderNameType.filename);
		if (newHash != shaderNameType.hash) {
			std::cout << "Detected changes in file: " << shaderNameType.filename << std::endl;
			std::cout << "New hash: " << shaderNameType.hash << std::endl;
			std::cout << "Recompile shader: " << shaderNameType.name << std::endl;

			reloadShader(shaderNameType.name);
		}
	}
}

std::string ShaderManager::hashForFile(std::string filename)
{
	MD5 md5;

	if (filename == "") {
		return "";
	}
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	if (size == 0)
	{
		return "";
	}

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		md5.update(&(buffer[0]), size);
		md5.finalize();
	}
	else
	{
		return "";
	}

	file.close();

	return md5.hexdigest();
}

void ShaderManager::reloadShader(std::string name)
{
	try {
		Resources::ShaderBuilder builder;
		for (int i = 0; i < Resources::SHADER_TYPE_NUM; ++i) {
			const std::string shaderNameTypeId = name + std::to_string(i);
			ShaderNameType shaderNameType = mapShaderfiles.at(shaderNameTypeId);
			if (shaderNameType.filename != "") {
				mapShaderfiles.at(shaderNameTypeId).hash = hashForFile(shaderNameType.filename);
				builder.AddShader(shaderNameType.filename, Resources::SHADER_TYPE(i));

			}
		}
	
		auto shaderPtr = builder.Build();
		shadersMap.erase(name);
		shadersMap.insert(std::make_pair <>(name, shaderPtr));
	}
	catch (...) {
		std::cout << "Cant reaload '" << name << " shader bacuase: ";// << ex.what() << std::endl;
		
	}
}
