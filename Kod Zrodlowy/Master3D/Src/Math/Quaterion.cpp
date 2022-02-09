#include "stdafx.h"

#include <Math\Quaterion.h>

Quaterion::Quaterion(float angle_rad, float x, float y, float z) {
	q[0] = cos(angle_rad / 2.0f);
	q[1] = sin(angle_rad / 2.0f)*x;
	q[2] = sin(angle_rad / 2.0f)*y;
	q[3] = sin(angle_rad / 2.0f)*z;
}

Quaterion::Quaterion(float angle_rad, Vec3 axis) : Quaterion(angle_rad, axis.v[0], axis.v[1], axis.v[2])
{

}

Quaterion::Quaterion() {
	q[0] = 0;
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
}

Quaterion Quaterion::operator + (const Quaterion& oth) const {
	Quaterion out;
	for (int i = 0; i < 4; ++i)
	{
		out.q[i] = oth.q[i] + q[i];
	}
	return out;
}
Quaterion Quaterion::operator - (const Quaterion& oth) const {
	Quaterion out;
	for (int i = 0; i < 4; ++i)
	{
		out.q[i] = q[i] - oth.q[i];
	}
	return out;
}
Quaterion Quaterion::operator * (const Quaterion& oth) const {
	Quaterion t;
	t.q[0] = q[0] * oth.q[0] - q[1] * oth.q[1] - q[2] * oth.q[2] - q[3] * oth.q[3];
	t.q[1] = q[1] * oth.q[0] - q[0] * oth.q[1] - q[3] * oth.q[2] - q[2] * oth.q[3];
	t.q[2] = q[2] * oth.q[0] - q[3] * oth.q[1] - q[0] * oth.q[2] - q[1] * oth.q[3];
	t.q[3] = q[3] * oth.q[0] - q[2] * oth.q[1] - q[1] * oth.q[2] - q[0] * oth.q[3];

	return t;
}
float Quaterion::dot(const Quaterion& oth) const {
	return q[0] * oth.q[0] + q[1] * oth.q[1] + q[2] * oth.q[2] + q[3] * oth.q[3];
}
Quaterion Quaterion::slerp(const Quaterion& q, const Quaterion& r, float t) {
	float dp = q.dot(r);

	if (fabs(dp) >= 1.0f) {
		return q;
	}

	float sin_omega = sqrt(1.0f - dp*dp);
	Quaterion res;
	if (fabs(sin_omega) < 0.001f)
	{
		for (int i = 0; i < 4; ++i) {
			res.q[i] = (1.0f - t) * q.q[i] + t*r.q[i];
		}
		return res;
	}

	float omega = acos(dp);
	float a = sin(1.0f - t * omega) / sin_omega;
	float b = sin(t * omega) / sin_omega;
	for (int i = 0; i < 4; ++i)
	{
		res.q[i] = q.q[i] * a + r.q[i] * b;
	}

	return res;
}

Quaterion Quaterion::normalise(const Quaterion &v) {
	float m = v.length();
	Quaterion res;
	res.q[0] = v.q[0] / m;
	res.q[1] = v.q[1] / m;
	res.q[2] = v.q[2] / m;
	res.q[3] = v.q[3] / m;
	return res;
}

Quaterion Quaterion::normalise() const {
	return Quaterion::normalise(*this);
}

Matrix4x4  Quaterion::getMatrix() const {
	/*float _v[16] =
	{
	1 - 2 * y * y - 2 * z*z, 2 * x*y - 2 * w*z, 2 * x*z + 2 * w*y, 0,
	2 * x*y + 2 * w*z, 1 - 2 * x*x - 2 * z*z, 2*y*z - 2 * w*x, 0,
	2 * x*z - 2 * w*y, 2*y*z + 2 * w * x, 1- 2*x*x - 2*y*y, 0,
	0,0,0,1
	}; */
	float _v[16] =
	{
		1 - 2 * q[2] * q[2] - 2 * q[3] * q[3], 2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[1] * q[3] + 2 * q[0] * q[2], 0,
		2 * q[1] * q[2] + 2 * q[0] * q[3], 1 - 2 * q[1] * q[1] - 2 * q[3] * q[3], 2 * q[2] * q[3] - 2 * q[0] * q[1], 0,
		2 * q[1] * q[3] - 2 * q[0] * q[2], 2 * q[2] * q[3] + 2 * q[0] * q[1], 1 - 2 * q[1] * q[1] - 2 * q[2] * q[2], 0,
		0, 0, 0, 1
	};
	return Matrix4x4(_v);
}

float Quaterion::length() const {
	return sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
}

