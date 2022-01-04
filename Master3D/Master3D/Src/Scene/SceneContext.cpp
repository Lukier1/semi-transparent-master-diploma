#include "stdafx.h"
#include <Scene/SceneContext.h>

Vec3 SceneContext::getAmbientLigtColor() const
{
	return mAmbientLightColor;
}

void SceneContext::setAmbientLightColor(const Vec3 & color)
{
	mAmbientLightColor = color;
}

