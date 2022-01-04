#include "stdafx.h"
#include "..\Include\GBufferAlpha.h"


GBufferAlpha::GBufferAlpha() : GBuffer(GBUFFER_ALPHA_TEX_TYPE_NUM_TEXTURES)
{

}


GBufferAlpha::~GBufferAlpha()
{
}

void GBufferAlpha::bindForWriting()
{
	GBuffer::bindForWriting();
	glBlendFuncSeparatei(GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA, GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	glBlendFuncSeparatei(GBUFFER_ALPHA_TEX_TYPE_BCK, GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE, GL_ONE);
	glBlendFuncSeparatei(GBUFFER_ALPHA_TEX_TYPE_DELTA, GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	

	GLfloat ACC_ALPHA_CLEAR[] = { 0,0,0,0 };
	GLfloat ACC_BCK_CLEAR[] = { 1,1,1,0 };
	GLfloat ACC_DELTA_CLEAR[] = { 0, 0 };
	glClearBufferfv(GL_COLOR, GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA, ACC_ALPHA_CLEAR);
	glClearBufferfv(GL_COLOR, GBUFFER_ALPHA_TEX_TYPE_BCK, ACC_BCK_CLEAR);
	glClearBufferfv(GL_COLOR, GBUFFER_ALPHA_TEX_TYPE_DELTA, ACC_DELTA_CLEAR);
}


bool GBufferAlpha::init(unsigned int windowW, unsigned int windowH)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(GBUFFER_NUM, mTextures);

	glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		windowW, windowH, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA, GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA], 0);

	glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_BCK]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		windowW, windowH, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_ALPHA_TEX_TYPE_BCK, GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_BCK], 0);

	glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_DELTA]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8_SNORM,
		windowW, windowH, 0,
		GL_RG, GL_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_ALPHA_TEX_TYPE_DELTA, GL_TEXTURE_2D, mTextures[GBUFFER_ALPHA_TEX_TYPE_DELTA], 0);


	glGenTextures(1, &mDepthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		windowW, windowH, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(GBUFFER_NUM, draw_bufs);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of framebuffers\n");
	}

	return true;
}
