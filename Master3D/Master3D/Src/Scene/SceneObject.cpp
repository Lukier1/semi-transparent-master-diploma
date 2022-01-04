#include "stdafx.h"
#include <Scene/SceneObject.h>

SceneObject::SceneObject() {
	mPositon = Vector3f(0, 0, 0);
}
SceneObject::SceneObject(const Vec3 &postion) {
	mPositon = postion;
}
void SceneObject::setPosition(const Vec3 & position)
{
	mPositon = position;
}

Vec3 SceneObject::getPosition() const
{
	return mPositon;
}
