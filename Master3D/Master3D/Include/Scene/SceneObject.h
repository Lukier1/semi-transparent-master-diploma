#pragma once

#include "Math\Vec.h"

class SceneObject {
public:
	SceneObject();
	SceneObject(const Vec3 &postion);
	void setPosition(const Vec3 &position);
	Vec3 getPosition() const ;
private:
	Vec3 mPositon;
};