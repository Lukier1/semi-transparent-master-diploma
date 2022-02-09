#pragma once

#include "../../stdafx.h"
#include "Vec.h"
#include "Matrix.h"


struct Quaterion {
	float q[4];

	Quaterion(float angle_rad, float x, float y, float z);
	Quaterion(float angle_rad, Vec3 axis);
	Quaterion();

	Quaterion operator +(const Quaterion& oth) const;
	Quaterion operator -(const Quaterion& oth) const;
	Quaterion operator *(const Quaterion& oth) const;
	float dot(const Quaterion& oth) const;
	static Quaterion slerp(const Quaterion& v1, const Quaterion& v2, float t);
	static Quaterion normalise(const Quaterion &v);
	Quaterion normalise() const;
	Matrix4x4 getMatrix() const;
	float length() const;

};