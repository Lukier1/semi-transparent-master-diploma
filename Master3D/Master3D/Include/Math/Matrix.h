#pragma once

#include "../../stdafx.h"
#include "Vec.h"

class Matrix4x4 {
private:
	float v[16];
public:
	Matrix4x4();
	Matrix4x4(float _v[16]);

	static Matrix4x4 translate(float x, float y, float z);
	static Matrix4x4 translate(Vec3 position);
	static Matrix4x4 scale(float x, float y, float z);
	static Matrix4x4 scale(Vec3 s);
	static Matrix4x4 viewMatrix(const Vec<3> &forw, const Vec<3> &right, const Vec<3> &eye, const Vec<3> &up );
	static Matrix4x4 viewMatrix(const Vec<3> &rot, const Vec<3> &pos);
	static Matrix4x4 lookat(const Vec<3> &eyepos, const Vec<3> &point, const Vec<3> &up = Vector3f(0, 1, 0));
	static Matrix4x4 rotation(float x, float y, float z);
	static Matrix4x4 rotation(Vec3 rot);
	static Matrix4x4 perspective(float _near, float _far, float fov, float aspect);
	static Matrix4x4 ortographic(float l, float b, float n, float r, float t, float f);
	static Matrix4x4 ortographic(Vec3 n1, Vec3 n2);
	const float * getMatrixData() const;
	Matrix4x4 operator +(const Matrix4x4& oth) const;
	Matrix4x4 operator -(const Matrix4x4& oth) const;
	Matrix4x4 operator *(const Matrix4x4& oth) const;

	Matrix4x4 traspose() const;
	Vec4 operator *(const Vec4& oth) const;
	//TODO: Get from smaller matrix

	Matrix4x4 inverse() const;

	std::string toString();
};