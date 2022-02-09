#pragma once

#include "SceneObject.h"
#include <Math\Matrix.h>

class SceneTransformObject : public SceneObject {
public:
	SceneTransformObject();
	Vec3 getRotation() const;
	void setRotation(const Vec3 &rotation);
	Vec3 getScale() const;
	void setScale(const Vec3 &scale);

	Matrix4x4 getTransmformationMatrix();
private:
	Vec3 mRotation;
	Vec3 mScale;
};