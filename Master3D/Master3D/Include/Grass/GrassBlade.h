#pragma once

#include "../../stdafx.h"

#include <Math\Vec.h>
#include <Resources\ShaderProgram.h>
#include <Resources\Texture.h>
#include <Scene/SceneObject.h>

class GrassBlade : public SceneObject {
public:
	GrassBlade();

	//TODO Enable chanign texture

	void setSize(Vec2 size);
	Vec2 getSize() const;

	void Render(const Matrix4x4 & V, const Matrix4x4 &P, const Resources::ShaderProgramPtr shader);

private:
	Vec2 mSize;

	Resources::Texture mTexture;
	GLuint mVAO;
};