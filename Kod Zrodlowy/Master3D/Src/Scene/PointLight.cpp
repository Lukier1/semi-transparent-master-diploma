#include "stdafx.h"
#include <Scene/PointLight.h>

PointLight::PointLight() 
{
	mPower = 1.0f;
}

PointLight::PointLight(const Vec3& pos) :PointLight() 
{
	setPosition(pos);
}

PointLight& PointLight::SetDiffusionColor(const Vec3 & diffusionColor)
{
	mDiffusionColor = diffusionColor;
	return *this;
}

Vec3 PointLight::GetDiffusionColor() const
{
	return mDiffusionColor;
}

PointLight& PointLight::SetReflectionColor(const Vec3 & reflectionColor)
{
	mReflectionColor = reflectionColor;
	return *this;
}

Vec3 PointLight::GetReflecitonColor() const
{
	return mReflectionColor;
}

PointLight& PointLight::SetPower(float power)
{
	mPower = power;
	return *this;
}

float PointLight::GetPower() const
{
	return mPower;
}

PointLight::PointLightOutput PointLight::GenerateOutput() const
{
	PointLightOutput output = {
		getPosition(),
		GetDiffusionColor(),
		GetReflecitonColor(),
		GetPower()
	};
	return output;
}
