#pragma once

#include "Math\Vec.h"
#include "Resources\Texture.h"
#include "Resources\ShaderProgram.h"
#include "Viewport.h"
class Quad2D {
public:
	Quad2D();
	void Init();

	enum QUAD_SHADERS {
		QUAD_SHADERS_TEXTURE_1B,
		QUAD_SHADERS_TEXTURE_3B,
		QUAD_SHADERS_BCK_ALPHA,
		QUAD_SHADER_DIRECT,
		QUAD_SHADER_NUM
	};

	void Render(GLuint texId, const Resources::ShaderProgram * shader, const Viewport& viewport);
	void Render(GLuint texId, const Resources::ShaderProgramPtr shader, const Viewport& viewport);
	void Render(GLuint texId, QUAD_SHADERS shader, const Viewport& viewport);
	void RenderMultisample(GLuint texId, const Resources::ShaderProgram * shader, const Viewport & viewport);

	void RenderVertices();

	void SetPostion(Vec2 position); ;
	Vec2 GetPosition() const;
private:
	Vec2 mPostion;
	
	GLuint mVAO;

	std::string mShaders[QUAD_SHADER_NUM];
};