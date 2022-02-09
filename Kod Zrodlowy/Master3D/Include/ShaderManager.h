#pragma once

#include "../stdafx.h"

#include "Resources\ShaderProgram.h"
#include "Singleton.h"
#include "Crypto\md5.h"

class ShaderManager : public Singleton<ShaderManager>  {
private:
	/*struct {
		Resources::ShaderProgramPtr shaders;

	};*/
	struct ShaderNameType {
		std::string filename;
		Resources::SHADER_TYPE type;
		std::string hash;
		std::string name;
	};
	std::map<std::string, ShaderNameType> mapShaderfiles;
	std::map<std::string, Resources::ShaderProgramPtr> shadersMap; 

	std::string hashForFile(std::string filename);
	void reloadShader(std::string name);

public:
	const Resources::ShaderProgram * getShader(std::string name);
	void putShader(std::string name, Resources::ShaderProgramPtr shaderPtr);
	
	void scan(float delta);
};