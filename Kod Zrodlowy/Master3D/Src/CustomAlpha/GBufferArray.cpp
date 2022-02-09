#include "stdafx.h"

#include <CustomAlpha/GBufferArray.h>

const int LAYER_COUNT = 8;

GBufferArray::GBufferArray() : GBufferAbstract(LAYER_COUNT) {

}


bool GBufferArray::init(unsigned int windowW, unsigned int windowH) {
	startInit();

	for (int i = 0; i < LAYER_COUNT; ++i) {
		CreateFBTexFloat(windowW, windowH, i);
	}


	GLenum draw_bufs[] = {  GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
							GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	glDrawBuffers(8, draw_bufs);

	checkInitStatus();
	return true;
}
 /*
void GBufferArray::init(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mArrayBuffer);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mArrayBuffer);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB,
		width, height,
		LAYER_COUNT, 0, GL_RGB, GL_UNSIGNED_INT, 0);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, mArrayBuffer);
	for (unsigned int i = 0; i < LAYER_COUNT; i++) {
		glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, mArrayBuffer, 0, i);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete gbuffer array framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of gbuffer array framebuffers\n");
	}
}


void GBufferArray::bindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void GBufferArray::bindForReading()
{
	bindForReading(0);
}

void GBufferArray::bindForReading(unsigned int offset)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, mArrayBuffer);

} */