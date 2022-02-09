#include "stdafx.h"

#include <Geometry\Plane.h>

using namespace Geometry;

const GLfloat QUAD_POINTS[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

Plane::Plane() {
	mAlpha = 1.0f;
	mSize = Vector2f(1, 1);
}
void Plane::Init() {
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_POINTS), &QUAD_POINTS, GL_STATIC_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Plane::Render(const Viewport &viewport, const Resources::ShaderProgram * program) {
	program->UseProgram();

	const Vec4 quadColor = Vector4f(mColor.v[0], mColor.v[1], mColor.v[2], mAlpha);
	program->apply("fig_color", quadColor);

	const Vec3 sizeWithZ = Vector3f(mSize.v[0], mSize.v[1], 1.0f);
	const Matrix4x4 M = Matrix4x4::translate(mPosition)*Matrix4x4::rotation(mRotation)*Matrix4x4::scale(sizeWithZ);
	const Matrix4x4 mvp = viewport.getPerspMat()*viewport.getViewMat()*M;

	program->apply("mvp_mat", mvp);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(QUAD_POINTS) / (3 * sizeof(GL_FLOAT)));
}


//TODO Wydzielic do innej klasy czeœæ metod
void Plane::SetSize(Vec2 size) {
	mSize = size;
}
Vec2 Plane::GetSize() const {
	return mSize;
}
void Plane::SetRotation(Vec3 rotation) {
	mRotation = rotation;
}
Vec3 Plane::GetRotation() const {
	return mRotation;
}

void Plane::setColor(Vec3 color) {
	mColor = color;
}
Vec3 Plane::getColor() const {
	return mColor;
}

void Plane::setAlpha(float alpha) {
	if (alpha > 1.0f) {
		mAlpha = 1.0f;
	}
	else if (alpha < 0.0f) {
		mAlpha = 0.0f;
	}
	mAlpha = alpha;
}
float Plane::getAlpha() const {
	return mAlpha;
}