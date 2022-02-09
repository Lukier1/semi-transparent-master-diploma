#pragma once

#include "CobMath.h"

class SceneContext {
public:
	Vec3 getAmbientLigtColor() const;
	void setAmbientLightColor(const Vec3 & color);
private:
	Vec3 mAmbientLightColor;
	
};