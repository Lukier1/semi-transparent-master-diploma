#include "stdafx.h"
#include <Resources\Material.h>
using namespace Resources;

void Material::SetParam(PARAM_TYPE type, const Vec3 & vec)
{
	switch (type) {
	case DIFFUSE_COLOR:
		break;
	case AMBIENT_COLOR:
		break;
	case SPECULAR_COLOR:
		break;
	default:
		std::cout << "Wrong type of param" << std::endl;
		return;//TODO: Wyjatek dodac
	}

	if (mVec3Map.count(type) > 0)
	{
		//Swap parameter
		mVec3Map[type] = vec;
	}
	else
	{
		//Add new 
		mVec3Map.insert(std::make_pair(type, vec));
	}

}

void Material::SetParam(PARAM_TYPE type, const float val)
{
	switch (type) {
	case SPEC_POWER:
		break;
	case OPACITY:
		break;
	default:
		std::cout << "Wrong type of param" << std::endl;
		return;//TODO: Wyjatek dodac
	}

	if (mFloatMap.count(type) > 0)
	{
		//Swap parameter
		mFloatMap[type] = val;
	}
	else
	{
		//Add new 
		mFloatMap.insert(std::make_pair(type, val));
	}
}

float Resources::Material::GetFloat(PARAM_TYPE type)
{
	return mFloatMap[type];
}

void Material::SetTexture(const Texture * tex)
{
	if (tex == nullptr)
	{
		Logger::getLogger()->Error("Cannot set texture for material, pointer is null");
		return;
	}
	if (mTextureMap.count(tex->getTextureType()) > 0)
	{
		//Swap parameter
		mTextureMap[tex->getTextureType()] = tex->getTexID();
	}
	else
	{
		//Add new 
		mTextureMap.insert(std::make_pair(tex->getTextureType(), tex->getTexID()));
	}
}

const std::string DIFFUSE_COLOR_STRING = "Ks";
const std::string AMBIENT_COLOR_STRING = "Kd";
const std::string SPECULAR_COLOR_STRING = "Ka";
const std::string SPEC_POWER_STRING = "spec_power";
const std::string SHADER_FLAG_STRING = "shader_flag";

void Material::ApplyParams(const ShaderProgram * program)
{
	GLuint shaderFlag = 0;
	for (auto iter = mVec3Map.begin(); iter != mVec3Map.end(); ++iter)
	{
		
		switch (iter->first) {
		case DIFFUSE_COLOR:
			program->apply(DIFFUSE_COLOR_STRING, iter->second);
			break;
		case AMBIENT_COLOR:
			program->apply(AMBIENT_COLOR_STRING, iter->second);
			break;
		case SPECULAR_COLOR:
			program->apply(SPECULAR_COLOR_STRING, iter->second);
			break;
		}
	}
	
	for (auto iter = mFloatMap.begin(); iter != mFloatMap.end(); ++iter)
	{

		switch (iter->first) {
		case SPEC_POWER:
			program->apply(SPEC_POWER_STRING, iter->second);
			break;
		}
	}
	
	for (auto iter = mTextureMap.begin(); iter != mTextureMap.end(); ++iter)
	{
		program->applyTexture(iter->first, iter->second);
		switch (iter->first) {
		case NORMALS_TEXTURE:
			shaderFlag = shaderFlag | NOMRALS_MAP_MODE;
			break;
		case SPECULAR_TEXTURE:
			shaderFlag = shaderFlag | SPECULAR_MAP_MODE;
			break;
		}
	}
	program->apply(SHADER_FLAG_STRING, shaderFlag);
}
