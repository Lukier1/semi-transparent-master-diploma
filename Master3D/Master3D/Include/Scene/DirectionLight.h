#pragma once

#include "CobMath.h"

class DirectionLight {
public:
	DirectionLight();

	DirectionLight& SetDiffusionColor(const Vec3& diffusionColor);
	Vec3 GetDiffusionColor() const;

	DirectionLight& SetReflectionColor(const Vec3& reflectionColor);
	Vec3 GetReflecitonColor() const;

	DirectionLight& SetDirection(const Vec3& direction);
	Vec3 GetDirection() const;
private:
	Vec3 mDiffusionColor;
	Vec3 mReflectionColor;
	Vec3 mDirection;
};