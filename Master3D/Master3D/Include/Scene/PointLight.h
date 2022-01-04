#pragma once

#include "CobMath.h"
#include "SceneObject.h"
#include "Billboard.h"
class PointLight :  public SceneObject {
public:
	PointLight();
	PointLight(const Vec3& pos);

	PointLight& SetDiffusionColor(const Vec3& diffusionColor);
	Vec3 GetDiffusionColor() const;

	PointLight& SetReflectionColor(const Vec3& reflectionColor);
	Vec3 GetReflecitonColor() const;

	PointLight& SetPower(float power);
	float GetPower() const;

	//DO usuniecia
	struct PointLightOutput {
		Vec3 position;
		Vec3 diffusionColor;
		Vec3 reflectionColor;
		float power;
	};
	//do usuniecia
	PointLightOutput GenerateOutput() const;
private:
	Vec3 mDiffusionColor;
	Vec3 mReflectionColor;

	float mPower;
	std::shared_ptr<Billboard> mBillboard;


};