#pragma once

#include "../stdafx.h"

#include "Viewport.h"
#include "Resources/ShaderProgram.h"
#include "Resources/Texture.h"
class SkyBox {
public:
	SkyBox();
	~SkyBox();

	void Render(const Viewport & viewport);

	void Render2(const Viewport& viewport);

	void Init();
	Resources::Texture mTexture;
private:
	std::shared_ptr<Resources::ShaderProgram> mShader;
	

	GLuint mVAO;
};