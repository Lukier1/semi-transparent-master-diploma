#include "stdafx.h"

#include<Quad2D.h>
#include <ShaderManager.h>

using namespace Resources;

const GLfloat QUAD_POINTS[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	-1.0f, 1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f
};
Quad2D::Quad2D()  {
	ShaderManager * shaderMgr = ShaderManager::GetSingletonPtr();
	

	mShaders[QUAD_SHADERS_TEXTURE_3B] = "QUAD_SHADER_TEXTURE_3B";
	mShaders[QUAD_SHADERS_TEXTURE_1B] = "QUAD_SHADERS_TEXTURE_1B";
	mShaders[QUAD_SHADERS_BCK_ALPHA] = "QUAD_SHADERS_BCK_ALPHA";

	shaderMgr->putShader(mShaders[QUAD_SHADERS_TEXTURE_3B],
		ShaderBuilder()
		.AddShader("Shaders/quad2d_vert.glsl", VERTEX)
		.AddShader("Shaders/quad2d_frag.glsl", FRAGMENT)
		.Build()
	);

	shaderMgr->putShader(mShaders[QUAD_SHADERS_TEXTURE_1B],
		ShaderBuilder()
		.AddShader("Shaders/quad2d_vert.glsl", VERTEX)
		.AddShader("Shaders/quad2d_1btex_frag.glsl", FRAGMENT)
		.Build()
	);

	shaderMgr->putShader(mShaders[QUAD_SHADERS_BCK_ALPHA],
		ShaderBuilder()
		.AddShader("Shaders/quad2d_vert.glsl", VERTEX)
		.AddShader("Shaders/deffered/AlphaBckPassFragm.glsl", FRAGMENT)
		.Build()
	);
	shaderMgr->putShader(mShaders[QUAD_SHADER_DIRECT],
		ShaderBuilder()
		.AddShader("Shaders/quad2d_vert.glsl", VERTEX)
		.AddShader("Shaders/CustomAlpha/quad2d_dir_tex_frag.glsl", FRAGMENT)
		.Build()
	);

}
void Quad2D::Init()  { 
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_POINTS) , &QUAD_POINTS, GL_STATIC_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}


void Quad2D::Render(GLuint texId, const Resources::ShaderProgram * shader, const Viewport & viewport)
{
	Vec2 scale = Vector2f(1, 1);
	Vec2 size = viewport.getViewSize();

	shader->UseProgram();

	shader->applyTexture(TextureType::DIFFUSE_TEXTURE, texId);
	shader->apply("scale", scale);
	shader->apply("texSize", size);


	RenderVertices();
}

void Quad2D::RenderMultisample(GLuint texId, const Resources::ShaderProgram * shader, const Viewport & viewport)
{
	Vec2 scale = Vector2f(1, 1);
	Vec2 size = viewport.getViewSize();

	shader->UseProgram();
	
	//shader->applyTexture(TextureType::DIFFUSE_TEXTURE, texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texId);
	shader->apply("scale", scale);
	shader->apply("texSize", size);
	shader->apply("phase", 0);

	RenderVertices();
}

void Quad2D::Render(GLuint texId, const Resources::ShaderProgramPtr shader, const Viewport & viewport)
{
	Vec2 scale = Vector2f(1, 1);
	Vec2 size = viewport.getViewSize();

	shader->UseProgram();

	shader->applyTexture(TextureType::DIFFUSE_TEXTURE, texId);
	shader->apply("scale", scale);
	shader->apply("texSize", size);


	RenderVertices();
}

void Quad2D::Render(GLuint texId, QUAD_SHADERS shader, const Viewport & viewport)
{
	if (shader == QUAD_SHADER_NUM)
	{
		Logger::getLogger()->Error("Cant use shader: [QUAD_SHADER_NUM]. ");
		return;
	}
	ShaderManager * shaderMgr = ShaderManager::GetSingletonPtr();

	Render(texId, shaderMgr->getShader(mShaders[shader]), viewport);
}

void Quad2D::RenderVertices() {
	glDepthMask(GL_FALSE);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDepthMask(GL_TRUE);
}

void Quad2D::SetPostion(Vec2 position) {
	mPostion = position;

}
Vec2 Quad2D::GetPosition() const {
	return mPostion;
}
