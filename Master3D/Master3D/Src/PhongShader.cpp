#include "stdafx.h"

#include <PhongShader.h>

PhongShader::PhongShader() {
	Resources::ShaderBuilder shaderBuilder;

	shaderBuilder.AddShader("Shaders/phong/light_vert.glsl", Resources::VERTEX);
	shaderBuilder.AddShader("Shaders/phong/light_frag.glsl", Resources::FRAGMENT);

	mShaderPtr = shaderBuilder.Build();

}

PhongShader::PhongShader(Resources::ShaderProgramPtr shaderPtr) {
	mShaderPtr = shaderPtr;
}

Resources::ShaderProgramPtr PhongShader::getShaderProgram() const
{
	return mShaderPtr;
}

void PhongShader::applyMVPMat(Matrix4x4 mvpMat)
{

	mShaderPtr->apply("mvp_mat", mvpMat);

}

void PhongShader::applyMVMat(Matrix4x4 mvMat)
{
	mShaderPtr->apply("mv_mat", mvMat);
}

void PhongShader::applyVMat(Matrix4x4 viewMat)
{
	mShaderPtr->apply("view_mat", viewMat);
}

void PhongShader::applyMMat(Matrix4x4 worldMat)
{
	mShaderPtr->apply("world_mat", worldMat);
}

void PhongShader::applyPointLight(PointLight pointLight)
{
	mShaderPtr->apply("LPos_world", pointLight.getPosition());
	mShaderPtr->apply("Ls", pointLight.GetReflecitonColor());
	mShaderPtr->apply("Ld", pointLight.GetDiffusionColor());
	mShaderPtr->apply("LPower", pointLight.GetPower()); 
}

void PhongShader::applyLa(Vec3 ambientColor)
{
	mShaderPtr->apply("La", ambientColor);
}


void PhongShader::applySpecPower(float specPower)
{
	mShaderPtr->apply("spec_power", specPower);
}

void PhongShader::use()
{
	mShaderPtr->UseProgram();
}
