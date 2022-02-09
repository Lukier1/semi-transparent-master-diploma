#include "stdafx.h"

#include <ShadowBuffer.h>

ShadowBuffer::ShadowBuffer() {
	mFbo = 0;
	mDepthTexture = 0;
}

ShadowBuffer::~ShadowBuffer()
{
	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mDepthTexture != 0) {
		glDeleteTextures(1, &mDepthTexture);
	}
}

void ShadowBuffer::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void ShadowBuffer::bindForReading()
{
	bindForReading(0);
}

void ShadowBuffer::bindForReading(unsigned int offset)
{
	glActiveTexture(GL_TEXTURE0+offset);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
}

GLuint ShadowBuffer::getDepthTexture()
{
	return mDepthTexture;
}

bool ShadowBuffer::init(unsigned int res)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mDepthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
		res, res, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//TODO do resa przywrocicc
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of shadow framebuffer\n");
	}

	return true;
}

GLuint ShadowBuffer::getFBO() const
{
	return mFbo;
}
