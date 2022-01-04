#include "stdafx.h"
#include <Scene\DirectionLight.h>

DirectionLight::DirectionLight() {
}

DirectionLight& DirectionLight::SetDiffusionColor(const Vec3& diffusionColor) {
	mDiffusionColor = diffusionColor;
	return *this;
}
Vec3 DirectionLight::GetDiffusionColor() const {
	return mDiffusionColor;
}

DirectionLight& DirectionLight::SetReflectionColor(const Vec3& reflectionColor) {
	mReflectionColor = reflectionColor;
	return *this;
}
Vec3 DirectionLight::GetReflecitonColor() const {
	return mReflectionColor;
}

DirectionLight& DirectionLight::SetDirection(const Vec3& direction) {
	mDirection = direction.normalizeVec3();
	return *this;
}
Vec3 DirectionLight::GetDirection() const {
	return mDirection;
}
