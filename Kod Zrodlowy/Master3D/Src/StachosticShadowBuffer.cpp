#include "stdafx.h"

#include <StachosticShadowBuffer.h>

StachosticShadowBuffer::StachosticShadowBuffer() {
	mFbo = 0;
	mDepthTexture = 0;
}

StachosticShadowBuffer::~StachosticShadowBuffer()
{
	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mDepthTexture != 0) {
		glDeleteTextures(1, &mDepthTexture);
	}
}

void StachosticShadowBuffer::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void StachosticShadowBuffer::bindForReading()
{
	bindForReading(0);
}

void StachosticShadowBuffer::bindForReading(unsigned int offset)
{
	glActiveTexture(GL_TEXTURE0+offset);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
	glActiveTexture(GL_TEXTURE0+offset+1);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mDepthTexture);
}

GLuint StachosticShadowBuffer::getDepthTexture()
{
	return mDepthTexture;
}

GLuint StachosticShadowBuffer::getTexture()
{
	return mTexture;
}

bool StachosticShadowBuffer::init(unsigned int res)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mTexture);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB16F,
		res, res, false);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture, 0);

	glGenTextures(1, &mDepthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mDepthTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_DEPTH_COMPONENT16,
		res, res, false);

	int maxSamples;
	glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxSamples);
	std::cout << "Max samples " << maxSamples << std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

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

GLuint StachosticShadowBuffer::getFBO() const
{
	return mFbo;
}

MultiSampleBuffer::MultiSampleBuffer()
{
	mFbo = 0;
	mMultiSampleDepthTex = 0;
	mMultiSampleTex = 0;
}

MultiSampleBuffer::~MultiSampleBuffer()
{
	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mMultiSampleTex != 0) {
		glDeleteTextures(1, &mMultiSampleTex);
	}
	
	if (mMultiSampleDepthTex != 0) {
		glDeleteTextures(1, &mMultiSampleDepthTex);
	}

}

void MultiSampleBuffer::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void MultiSampleBuffer::bindForReading(unsigned int offset)
{
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mMultiSampleTex);
	glActiveTexture(GL_TEXTURE0 + offset + 1);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mMultiSampleDepthTex);
}

bool MultiSampleBuffer::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mMultiSampleTex);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mMultiSampleTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB16F,
		width, height, false);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mMultiSampleTex, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of MultiSampleBuffer framebuffer\n");
	}

	size.v[0] = width;
	size.v[1] = height;

	return true;
}

GLuint MultiSampleBuffer::getMSTexture() const
{
	return mMultiSampleTex;
}

GLuint MultiSampleBuffer::getFBO() const
{
	return mFbo;
}

SingleTexBuffer::SingleTexBuffer()
{
	mFbo = 0;
	mTexture = 0;
	mDepthTexture = 0;
}

SingleTexBuffer::~SingleTexBuffer()
{
	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mTexture != 0) {
		glDeleteTextures(1, &mTexture);
	}

	if (mDepthTexture != 0) {
		glDeleteTextures(1, &mDepthTexture);
	}
}

void SingleTexBuffer::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void SingleTexBuffer::bindForReading(unsigned int offset)
{
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glActiveTexture(GL_TEXTURE0 + offset + 1);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
}

bool SingleTexBuffer::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mTexture);

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		width, height, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

	glGenTextures(1, &mDepthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufs);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of SingleTexBuffer\n");
	}

	return true;
}

GLuint SingleTexBuffer::getTexture()
{
	return mTexture;
}

GLuint SingleTexBuffer::getFBO() const
{
	return mFbo;
}
