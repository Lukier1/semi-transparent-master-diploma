#include "stdafx.h"
#include <Math\Matrix.h>

Matrix4x4::Matrix4x4() {
	v[0] = 1.0f;
	v[5] = 1.0f;
	v[10] = 1.0f;
	v[15] = 1.0f;
}
Matrix4x4::Matrix4x4(float _v[16])
{
	float out[] = { _v[0], _v[4], _v[8], _v[12], _v[1], _v[5], _v[9], _v[13], _v[2], _v[6], _v[10], _v[14], _v[3], _v[7], _v[11], _v[15] };
	for (int i = 0; i < 16; ++i)
	{
		v[i] = _v[i];
	}
}

//Not transpose
Matrix4x4 Matrix4x4::translate(float x, float y, float z)
{
	float _v[16] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1 };
	return Matrix4x4(_v).traspose();
}
Matrix4x4 Matrix4x4::translate(Vec3 position)
{
	return translate(position.v[0], position.v[1], position.v[2]);
}

//N/A
Matrix4x4 Matrix4x4::scale(float x, float y, float z)
{
	float _v[16] = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1 };

	return Matrix4x4(_v);
}
Matrix4x4 Matrix4x4::scale(Vec3 s)
{
	return scale(s.v[0], s.v[1], s.v[2]);
}

//Not trnapose
Matrix4x4 Matrix4x4::viewMatrix(const Vec<3> &rot, const Vec<3> &eyepos) {
	Matrix4x4 rotMat = rotation(rot.v[0], rot.v[1], rot.v[2]);
	
	Vec3 s = Vector3f(rotMat.getMatrixData()[0], rotMat.getMatrixData()[4], rotMat.getMatrixData()[8]);
	Vec3 u = Vector3f(rotMat.getMatrixData()[1], rotMat.getMatrixData()[5], rotMat.getMatrixData()[9]);
	Vec3 f = Vector3f(rotMat.getMatrixData()[2], rotMat.getMatrixData()[6], rotMat.getMatrixData()[10]);

	float T[16] =
	{
		1, 0, 0, -eyepos.v[0],
		0, 1, 0, -eyepos.v[1],
		0, 0, 1, -eyepos.v[2],
		0, 0, 0, 1
	};
	float M[16] =
	{
		s.v[0], s.v[1], s.v[2], 0,
		u.v[0], u.v[1], u.v[2], 0,
		-f.v[0], -f.v[1], -f.v[2], 0,
		0, 0, 0, 1
	};
	return (Matrix4x4(M)*Matrix4x4(T)).traspose();
}

//Not transpose
Matrix4x4 Matrix4x4::viewMatrix(const Vec<3> &forw, const Vec<3> &right, const Vec<3> &eye, const Vec<3> &up) {
	float _v[16] =
	{
		right.v[0], right.v[1], right.v[2], -eye.v[0],
		up.v[0], up.v[1], up.v[2], -eye.v[1],
		-forw.v[0], -forw.v[1], -forw.v[2], -eye.v[2],
		0, 0, 0, 1
	};
	
	return Matrix4x4(_v).traspose();
}

//Not tranpose
Matrix4x4 Matrix4x4::lookat(const Vec<3> &eyepos, const Vec<3> &point, const Vec<3> &up) {
	Vec3 zaxis;
	zaxis = eyepos - point;
	zaxis = zaxis.normalizeVec3();
	Vec3 xaxis = up.normalizeVec3().cross(zaxis).normalizeVec3();
	Vec3 yaxis = zaxis.cross(xaxis).normalizeVec3();


	float M[16] =
	{
		xaxis.v[0], xaxis.v[1], xaxis.v[2], -xaxis.dot(eyepos),
		yaxis.v[0], yaxis.v[1], yaxis.v[2], -yaxis.dot(eyepos),
		-zaxis.v[0], -zaxis.v[1], -zaxis.v[2], -zaxis.dot(eyepos),
		0,0,0,1
	};
	return (Matrix4x4(M).traspose());

}

//Rather not tranpose
Matrix4x4 Matrix4x4::rotation(float x, float y, float z)
//x - alpha
//y - beta
//z - gamma
{
	const float cos_z = cos(z);
	const float sin_z = sin(z);
	const float sin_x = sin(x);
	const float cos_x = cos(x);
	const float cos_y = cos(y);
	const float sin_y = sin(y);

	float _v[16] = {
		cos_y*cos_z, cos_z*sin_x*sin_y - cos_x*sin_z, cos_x*cos_z*sin_y + sin_x*sin_z, 0,
		cos_y*sin(z), cos_x*cos_z + sin_x*sin_y*sin_z, -cos(z)*sin_x + cos_x*sin_y*sin_z, 0,
		-sin_y, cos_y*sin_x, cos_x*cos_y, 0,
		0, 0, 0, 1 };
	return Matrix4x4(_v);
}

Matrix4x4 Matrix4x4::rotation(Vec3 rot)
{
	return rotation(rot.v[0] ,rot.v[1], rot.v[2]);
}

//No transpose
Matrix4x4 Matrix4x4::perspective(float near_per, float far_per, float fov, float aspect) {
	//TODO: Check for range == 0
	float range = tan(fov*0.5f);
	/*float _v[16] = {
		near_per / (range * aspect), 0, 0, 0,
		0, near_per / range, 0, 0,
		0, 0, -2 / (far_per - near_per), -(far_per + near_per) / (far_per - near_per),
		0, 0, -1, 0
	}; */
	float _v2[16] = {
		-1 / (range * aspect), 0, 0, 0,
		0, 1 / range, 0, 0,
		0, 0, -(far_per + near_per) / (far_per - near_per), -1,
		0, 0, -2*(far_per*near_per) / (far_per - near_per) , 0
	};
	return Matrix4x4(_v2);
}

//After transpose
Matrix4x4 Matrix4x4::ortographic(float l, float b, float n, float r, float t, float f)
{
	float _v[16] = {
		2.0f/(r-l), 0, 0, 0,
		0, 2.0f/(t-b), 0, 0,
		0, 0, -2.0f/(f-n), 0,
		-(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1
	};
	return Matrix4x4(_v);
}

Matrix4x4 Matrix4x4::ortographic(Vec3 n1, Vec3 n2)
{
	return ortographic(n1.v[0], n1.v[1], n1.v[2], n2.v[0], n2.v[1], n2.v[2]);
}

const float * Matrix4x4::getMatrixData() const {
	return v;
}


Matrix4x4 Matrix4x4::operator+(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	for (int i = 0; i < 16; ++i)
	{
		res.v[i] = v[i] + oth.v[i];
	}
	return res;
}
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	for (int i = 0; i < 16; ++i)
	{
		res.v[i] = v[i] - oth.v[i];
	}
	return res;
}
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	res.v[0] = v[0] * oth.v[0] + v[1] * oth.v[4] + v[2] * oth.v[8] + v[3] * oth.v[12];
	res.v[1] = v[0] * oth.v[1] + v[1] * oth.v[5] + v[2] * oth.v[9] + v[3] * oth.v[13];
	res.v[2] = v[0] * oth.v[2] + v[1] * oth.v[6] + v[2] * oth.v[10] + v[3] * oth.v[14];
	res.v[3] = v[0] * oth.v[3] + v[1] * oth.v[7] + v[2] * oth.v[11] + v[3] * oth.v[15];

	res.v[4] = v[4] * oth.v[0] + v[5] * oth.v[4] + v[6] * oth.v[8] + v[7] * oth.v[12];
	res.v[5] = v[4] * oth.v[1] + v[5] * oth.v[5] + v[6] * oth.v[9] + v[7] * oth.v[13];
	res.v[6] = v[4] * oth.v[2] + v[5] * oth.v[6] + v[6] * oth.v[10] + v[7] * oth.v[14];
	res.v[7] = v[4] * oth.v[3] + v[5] * oth.v[7] + v[6] * oth.v[11] + v[7] * oth.v[15];

	res.v[8] = v[8] * oth.v[0] + v[9] * oth.v[4] + v[10] * oth.v[8] + v[11] * oth.v[12];
	res.v[9] = v[8] * oth.v[1] + v[9] * oth.v[5] + v[10] * oth.v[9] + v[11] * oth.v[13];
	res.v[10] = v[8] * oth.v[2] + v[9] * oth.v[6] + v[10] * oth.v[10] + v[11] * oth.v[14];
	res.v[11] = v[8] * oth.v[3] + v[9] * oth.v[7] + v[10] * oth.v[11] + v[11] * oth.v[15];

	res.v[12] = v[12] * oth.v[0] + v[13] * oth.v[4] + v[14] * oth.v[8] + v[15] * oth.v[12];
	res.v[13] = v[12] * oth.v[1] + v[13] * oth.v[5] + v[14] * oth.v[9] + v[15] * oth.v[13];
	res.v[14] = v[12] * oth.v[2] + v[13] * oth.v[6] + v[14] * oth.v[10] + v[15] * oth.v[14];
	res.v[15] = v[12] * oth.v[3] + v[13] * oth.v[7] + v[14] * oth.v[11] + v[15] * oth.v[15];

	return res;
}

Vec4 Matrix4x4::operator*(const Vec4& oth) const
{
	return Vec4(v[0] * oth.v[0] + v[1] * oth.v[1] + v[2] * oth.v[2] + v[3] * oth.v[3],
		v[4] * oth.v[0] + v[5] * oth.v[1] + v[6] * oth.v[2] + v[7] * oth.v[3],
		v[8] * oth.v[0] + v[9] * oth.v[1] + v[10] * oth.v[2] + v[11] * oth.v[3],
		v[12] * oth.v[0] + v[13] * oth.v[1] + v[14] * oth.v[2] + v[15] * oth.v[3]);
}
Matrix4x4 Matrix4x4::traspose() const
{
	float out_v[] = { v[0], v[4], v[8], v[12], v[1], v[5], v[9], v[13], v[2], v[6], v[10], v[14], v[3], v[7], v[11], v[15] };
	return Matrix4x4(out_v);
}

Matrix4x4 Matrix4x4::inverse() const
{
	float inv[16], det;

	inv[0] = v[5] * v[10] * v[15] -
		v[5] * v[11] * v[14] -
		v[9] * v[6] * v[15] +
		v[9] * v[7] * v[14] +
		v[13] * v[6] * v[11] -
		v[13] * v[7] * v[10];

	inv[4] = -v[4] * v[10] * v[15] +
		v[4] * v[11] * v[14] +
		v[8] * v[6] * v[15] -
		v[8] * v[7] * v[14] -
		v[12] * v[6] * v[11] +
		v[12] * v[7] * v[10];

	inv[8] = v[4] * v[9] * v[15] -
		v[4] * v[11] * v[13] -
		v[8] * v[5] * v[15] +
		v[8] * v[7] * v[13] +
		v[12] * v[5] * v[11] -
		v[12] * v[7] * v[9];

	inv[12] = -v[4] * v[9] * v[14] +
		v[4] * v[10] * v[13] +
		v[8] * v[5] * v[14] -
		v[8] * v[6] * v[13] -
		v[12] * v[5] * v[10] +
		v[12] * v[6] * v[9];

	inv[1] = -v[1] * v[10] * v[15] +
		v[1] * v[11] * v[14] +
		v[9] * v[2] * v[15] -
		v[9] * v[3] * v[14] -
		v[13] * v[2] * v[11] +
		v[13] * v[3] * v[10];

	inv[5] = v[0] * v[10] * v[15] -
		v[0] * v[11] * v[14] -
		v[8] * v[2] * v[15] +
		v[8] * v[3] * v[14] +
		v[12] * v[2] * v[11] -
		v[12] * v[3] * v[10];

	inv[9] = -v[0] * v[9] * v[15] +
		v[0] * v[11] * v[13] +
		v[8] * v[1] * v[15] -
		v[8] * v[3] * v[13] -
		v[12] * v[1] * v[11] +
		v[12] * v[3] * v[9];

	inv[13] = v[0] * v[9] * v[14] -
		v[0] * v[10] * v[13] -
		v[8] * v[1] * v[14] +
		v[8] * v[2] * v[13] +
		v[12] * v[1] * v[10] -
		v[12] * v[2] * v[9];

	inv[2] = v[1] * v[6] * v[15] -
		v[1] * v[7] * v[14] -
		v[5] * v[2] * v[15] +
		v[5] * v[3] * v[14] +
		v[13] * v[2] * v[7] -
		v[13] * v[3] * v[6];

	inv[6] = -v[0] * v[6] * v[15] +
		v[0] * v[7] * v[14] +
		v[4] * v[2] * v[15] -
		v[4] * v[3] * v[14] -
		v[12] * v[2] * v[7] +
		v[12] * v[3] * v[6];

	inv[10] = v[0] * v[5] * v[15] -
		v[0] * v[7] * v[13] -
		v[4] * v[1] * v[15] +
		v[4] * v[3] * v[13] +
		v[12] * v[1] * v[7] -
		v[12] * v[3] * v[5];

	inv[14] = -v[0] * v[5] * v[14] +
		v[0] * v[6] * v[13] +
		v[4] * v[1] * v[14] -
		v[4] * v[2] * v[13] -
		v[12] * v[1] * v[6] +
		v[12] * v[2] * v[5];

	inv[3] = -v[1] * v[6] * v[11] +
		v[1] * v[7] * v[10] +
		v[5] * v[2] * v[11] -
		v[5] * v[3] * v[10] -
		v[9] * v[2] * v[7] +
		v[9] * v[3] * v[6];

	inv[7] = v[0] * v[6] * v[11] -
		v[0] * v[7] * v[10] -
		v[4] * v[2] * v[11] +
		v[4] * v[3] * v[10] +
		v[8] * v[2] * v[7] -
		v[8] * v[3] * v[6];

	inv[11] = -v[0] * v[5] * v[11] +
		v[0] * v[7] * v[9] +
		v[4] * v[1] * v[11] -
		v[4] * v[3] * v[9] -
		v[8] * v[1] * v[7] +
		v[8] * v[3] * v[5];

	inv[15] = v[0] * v[5] * v[10] -
		v[0] * v[6] * v[9] -
		v[4] * v[1] * v[10] +
		v[4] * v[2] * v[9] +
		v[8] * v[1] * v[6] -
		v[8] * v[2] * v[5];

	det = v[0] * inv[0] + v[1] * inv[4] + v[2] * inv[8] + v[3] * inv[12];

	if (det == 0)
		return *this;

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return Matrix4x4(inv);
}

std::string Matrix4x4::toString()
{
	std::string out = "";
	for (int i = 0; i < 4; ++i) {
		out += "|" + std::to_string(v[i * 4]) + ", " + std::to_string(v[i * 4 + 1]) + ", " + std::to_string(v[i * 4 + 2]) + ", " + std::to_string(v[i * 4 + 3]) + "|\n";
	}
	return out;
}
