#include "stdafx.h"

#include <GBuffer.h>

GBuffer::GBuffer() : GBuffer((int)GBUFFER_NUM_TEXTURES)
{

}

GBuffer::GBuffer(const int numberOfBuffer) : GBUFFER_NUM(numberOfBuffer) {
	mFbo = 0;
	mDepthTexture = 0;

	mTextures = new GLuint[numberOfBuffer];
	ZeroMemory(mTextures, sizeof(mTextures));
}

GBuffer::~GBuffer()
{

	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mTextures[0] != 0) {
		glDeleteTextures(GBUFFER_NUM_TEXTURES, mTextures); 
		delete[] mTextures;
	}

	if (mDepthTexture != 0) {
		glDeleteTextures(1, &mDepthTexture);
	}
}

bool GBuffer::init(unsigned int windowW, unsigned int windowH)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(GBUFFER_NUM_TEXTURES, mTextures);

	CreateFBTexFloat(windowW, windowH, 0);
	CreateFBTex(windowW, windowH, 1);
	CreateFBTexFloat(windowW, windowH, 2);
	CreateFBTex(windowW, windowH, 3);

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

	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, draw_bufs);

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

void GBuffer::CreateFBTexFloat(unsigned int windowW, unsigned int windowH, int num) {
	glBindTexture(GL_TEXTURE_2D, mTextures[num]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		windowW, windowH, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D, mTextures[num], 0);
}

void GBuffer::CreateFBTex(unsigned int windowW, unsigned int windowH, int num) {
	glBindTexture(GL_TEXTURE_2D, mTextures[num]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		windowW, windowH, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_TEXTURE_2D, mTextures[num], 0);
}

void GBuffer::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void GBuffer::bindForReading()
{
	bindForReading(0);
}

void GBuffer::bindForReading(unsigned int offset)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (unsigned int i = 0; i < GBUFFER_NUM; i++) {
		glActiveTexture(GL_TEXTURE0 + i + offset);
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
	}
	
	//TODO Opytmalizacja, czy zawsze potrzeba bufora g³êbi?
	glActiveTexture(GL_TEXTURE0 + GBUFFER_NUM + offset);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
}

GLuint GBuffer::getTexture(GBUFFER_TEX_TYPE bufferTexType)
{
	if (bufferTexType == GBUFFER_NUM_TEXTURES) {
		return 0;
	}
	return mTextures[bufferTexType];
}

GLuint GBuffer::getDepthTexture()
{
	return mDepthTexture;
}
