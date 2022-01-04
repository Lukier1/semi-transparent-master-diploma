#include "stdafx.h"

#include <SkyBox.h>

using namespace Resources;

GLfloat CUBE_POINTS[108] =
{
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f
};

SkyBox::SkyBox() :/*
	mTexture("Data/Images/lake_sky/lake1_bk.jpg", "Data/Images/lake_sky/lake1_ft.jpg", 
		"Data/Images/lake_sky/lake1_up.jpg", "Data/Images/lake_sky/lake1_dn.jpg", 
		"Data/Images/lake_sky/lake1_lf.jpg", "Data/Images/lake_sky/lake1_rt.jpg")*/
	mTexture("Data/Images/field/px.png", "Data/Images/field/nx.png",
		"Data/Images/field/py.png", "Data/Images/field/ny.png",
		"Data/Images/field/pz.png", "Data/Images/field/nz.png")
{
	mVAO = 0;
	
	ShaderBuilder shaderBuilder;
	shaderBuilder.AddShader("Shaders/skybox_vert.glsl", Resources::VERTEX);
	shaderBuilder.AddShader("Shaders/skybox_frag.glsl", Resources::FRAGMENT);
	mShader = shaderBuilder.Build();
}

SkyBox::~SkyBox()
{
}

void SkyBox::Render(const Viewport & viewport)
{
	/*
	glDepthMask(GL_FALSE);
	mShader->UseProgram();

	mShader->applyTexture(mTexture.getTextureType(), mTexture.getTexID());
	mShader->apply("P", viewport.getPerspMat());
	mShader->apply("V", viewport.getViewMat());
	*/
	//mShader->UseProgram();
	glDepthMask(GL_FALSE);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void SkyBox::Render2(const Viewport& viewport)
{
	glDepthMask(GL_FALSE);
	mShader->UseProgram();

	mShader->applyTexture(mTexture.getTextureType(), mTexture.getTexID());
	mShader->apply("P", viewport.getPerspMat());
	mShader->apply("V", viewport.getViewMat());
	
	glDepthMask(GL_FALSE);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
}

void SkyBox::Init()
{

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, 108*4, &CUBE_POINTS, GL_STATIC_DRAW);

	mVAO = 0;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
