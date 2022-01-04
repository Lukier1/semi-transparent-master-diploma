#include "stdafx.h"

#include <Math\Vec.h>

Vec4 Vector4f(float x, float y, float z, float w)
{
	float v[] = { x, y, z, w};
	return Vec4(v);
}

Vec3 Vector3f(float x, float y, float z) {
	float v[] = { x, y, z };
	return Vec3(v);
}

Vec2 Vector2f(float x, float y)
{
	float v[] = { x, y };
	return Vec2(v);
}

