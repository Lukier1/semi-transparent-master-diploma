#include "stdafx.h"
#include <Billboard.h>

using namespace Resources;
const GLfloat CUBE_POINTS[6] =
{
	-0.0f,  0.0f, 0.0f
};

Billboard::Billboard() : mTexture("Data/Images/box.png", DIFFUSE_TEXTURE)
{
	ShaderBuilder builder;
	mShader = builder
		.AddShader("Shaders/geomVert.glsl", VERTEX)
		.AddShader("Shaders/geomFrag.glsl", FRAGMENT)
		.AddShader("Shaders/bill_geom.glsl", GEOMETRY)
		.Build();

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_POINTS), &CUBE_POINTS, GL_STATIC_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	mSize = Vector2f(5, 5);

}

void Billboard::setSize(Vec2 size)
{
	mSize = size;
}

Vec2 Billboard::getSize() const
{
	return mSize;
}

void Billboard::Render(const Matrix4x4 & V, const Matrix4x4 &P)
{
	mShader->UseProgram();

	mShader->applyTexture(mTexture.getTextureType(), mTexture.getTexID());
	Matrix4x4 mvp;
	Matrix4x4 M = Matrix4x4::translate(getPosition());
	mvp = P*V*M;
	mShader->apply("mvp_mat", mvp);
	mShader->apply("P_mat", P);


	glBindVertexArray(mVAO);
	glDrawArrays(GL_POINTS, 0, sizeof(CUBE_POINTS) / (3 * sizeof(GLfloat)));
}
