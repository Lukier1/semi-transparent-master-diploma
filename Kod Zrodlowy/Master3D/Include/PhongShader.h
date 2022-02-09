#pragma once

#include "Resources\ShaderProgram.h"
#include "Scene/PointLight.h"
class PhongShader {
public:
	PhongShader();
	PhongShader(Resources::ShaderProgramPtr shaderPtr);

	Resources::ShaderProgramPtr getShaderProgram() const;

	void applyMVPMat(Matrix4x4 mvpMat);
	void applyMVMat(Matrix4x4 mvMat);
	void applyVMat(Matrix4x4 viewMat);
	void applyMMat(Matrix4x4 worldMat);
	void applyPointLight(PointLight pointLight);
	void applyLa(Vec3 ambientColor);
	void applySpecPower(float specPower);
	void use();

private:
	Resources::ShaderProgramPtr mShaderPtr;
};
/*
mShaderProgram->apply("LPos_world", mPointLight.getPosition());
mShaderProgram->apply("Ls", mPointLight.GetReflecitonColor());
mShaderProgram->apply("Ld", mPointLight.GetDiffusionColor());
mShaderProgram->apply("LPower", mPointLight.GetPower());*/