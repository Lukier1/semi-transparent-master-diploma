#include "stdafx.h"

#include <Scene\SceneTransformObject.h>

SceneTransformObject::SceneTransformObject() {
	mRotation = Vector3f(0, 0, 0);
	mScale = Vector3f(1, 1, 1);
}

Vec3 SceneTransformObject::getRotation() const
{
	return mRotation;
}

void SceneTransformObject::setRotation(const Vec3 & rotation)
{
	mRotation = rotation;
}

Vec3 SceneTransformObject::getScale() const
{
	return mScale;
}

void SceneTransformObject::setScale(const Vec3 & scale)
{
	mScale = scale;
}

Matrix4x4 SceneTransformObject::getTransmformationMatrix()
{
	return Matrix4x4::scale(getScale()) * Matrix4x4::translate(getPosition())*Matrix4x4::rotation(getRotation());
}
