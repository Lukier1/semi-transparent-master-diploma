#pragma once

#include "../stdafx.h"

#include "Math\Vec.h"
#include "Resources\ShaderProgram.h"
#include "Resources\Texture.h"
#include "SceneObject.h"

class Billboard : public SceneObject {
public:
	Billboard();

	//TODO Enable chanign texture

	void setSize(Vec2 size);
	Vec2 getSize() const;

	void Render(const Matrix4x4 & V, const Matrix4x4 &P);

private:
	Vec2 mSize; 

	Resources::Texture mTexture;
	Resources::ShaderProgramPtr mShader;

	GLuint mVAO;
};