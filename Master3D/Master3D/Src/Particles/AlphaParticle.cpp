#include "stdafx.h"

#include <Particles/AlphaParticle.h>

using namespace Particles;
using namespace Resources;
std::vector<GLfloat> positions;
//Move to factory
const GLfloat QUAD_POINTS[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

const GLfloat QUAD_NORMALS[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

//Move to factory
const GLfloat QUAD_TEXCOORDS[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
};

AlphaParticle::AlphaParticle(Resources::Texture &texture) : mTexture(texture) {
	mAlpha = 1.0f;
	mSize = Vector2f(1, 1);
	setPosition(Vector3f(0, 4, 0));
}

void AlphaParticle::Init() {
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_POINTS), &QUAD_POINTS, GL_STATIC_DRAW);

	GLuint vboN = 0;
	glGenBuffers(1, &vboN);
	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_NORMALS), &QUAD_NORMALS, GL_STATIC_DRAW);

	GLuint vboT = 0;
	glGenBuffers(1, &vboT);
	glBindBuffer(GL_ARRAY_BUFFER, vboT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_TEXCOORDS), &QUAD_TEXCOORDS, GL_STATIC_DRAW);

	//Position buffer
	int seed = 21412;
	srand(seed);
	for (int i = 0; i < 1000; ++i) {
		positions.push_back((rand() % 10000)*0.0005);
		positions.push_back((rand() % 10000)*0.0005);
		positions.push_back((rand() % 10000)*0.0005);
	}

	GLuint vboP = 0;
	glGenBuffers(1, &vboP);
	glBindBuffer(GL_ARRAY_BUFFER, vboP);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*positions.size(), &(positions[0]), GL_STREAM_DRAW);
	
	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribBinding(1, 1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vboT);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribBinding(2, 2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vboP);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribBinding(3, 3);

	glVertexAttribDivisor(0, 0); 
	glVertexAttribDivisor(1, 0); 
	glVertexAttribDivisor(2, 0); 
	glVertexAttribDivisor(3, 1);
}
void AlphaParticle::RenderMulti(const Matrix4x4& perspMat, const Matrix4x4& viewMat,  const Resources::ShaderProgram * program) {
	
	program->UseProgram();

	const float eta = 0.99;
	const Vec3 transmitive = Vector3f(1.0,1.0,1.0);
	program->apply("transmitive", transmitive);
	program->apply("eta", eta);
	program->apply("alpha", mAlpha);
	program->apply("ambientLight", Vector3f(1, 1, 1));
	program->apply("isTextured", 1);
	program->apply("color", Vector3f(1, 0, 1));
	
	const Vec3 sizeWithZ = Vector3f(mSize.v[0], mSize.v[1], 1.0f);


	int seed = 21412;
	srand(seed);

	const Matrix4x4 M = Matrix4x4::translate(getPosition())*Matrix4x4::rotation(mRotation)*Matrix4x4::scale(sizeWithZ);
	const Matrix4x4 MV = M* viewMat;
	const Matrix4x4 MVP = MV*perspMat;

	program->applyTexture(TextureType::DIFFUSE_TEXTURE, mTexture.getTexID());

	program->apply("PMat", perspMat);
	program->apply("MMat", M);
	program->apply("VMat", viewMat);
	program->apply("MVMat", MV);
	program->apply("MVPMat", MVP);

	glBindVertexArray(mVAO);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(QUAD_POINTS) / (3 * sizeof(GL_FLOAT)));
	static int count = 50;
	++count;
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, sizeof(QUAD_POINTS) / (3 * sizeof(GL_FLOAT)), int(count/20.0));
	
}

void AlphaParticle::Render(const Matrix4x4& perspMat, const Matrix4x4& viewMat, const Resources::ShaderProgram* program) {
	program->UseProgram();

	const float eta = 0.96;
	const Vec3 transmitive = Vector3f(1.0, 1.0, 1.0);
	program->apply("transmitive", transmitive);
	program->apply("eta", eta);
	program->apply("alpha", mAlpha);
	program->apply("ambientLight", Vector3f(1, 1, 1));
	program->apply("isTextured", 1);
	program->apply("color", Vector3f(1, 0, 1));


	const Vec3 sizeWithZ = Vector3f(mSize.v[0], mSize.v[1], 1.0f);
	const Matrix4x4 M = Matrix4x4::translate(getPosition()) * Matrix4x4::rotation(mRotation) * Matrix4x4::scale(sizeWithZ);
	const Matrix4x4 MV = M * viewMat;
	const Matrix4x4 MVP = MV * perspMat;

	program->applyTexture(TextureType::DIFFUSE_TEXTURE, mTexture.getTexID());
	program->apply("MMat", M);
	program->apply("PMat", perspMat);
	program->apply("MVMat", MV);
	program->apply("MVPMat", MVP);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(QUAD_POINTS) / (3 * sizeof(GL_FLOAT)));

}

void AlphaParticle::Render(const Viewport &viewport, const Resources::ShaderProgram * program) {
	Render(viewport.getPerspMat(), viewport.getViewMat(), program);
}


//TODO Wydzielic do innej klasy czeœæ metod
void AlphaParticle::SetSize(Vec2 size) {
	mSize = size;
}
Vec2 AlphaParticle::GetSize() const {
	return mSize;
}
void AlphaParticle::SetRotation(Vec3 rotation) {
	mRotation = rotation;
}
Vec3 AlphaParticle::GetRotation() const {
	return mRotation;
}

void AlphaParticle::setAlpha(float alpha) {
	if (alpha > 1.0f) {
		mAlpha = 1.0f;
	}
	else if (alpha < 0.0f) {
		mAlpha = 0.0f;
	}
	mAlpha = alpha;
}
float AlphaParticle::getAlpha() const {
	return mAlpha;
}