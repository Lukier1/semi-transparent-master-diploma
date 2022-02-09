#include "stdafx.h"

#include <CustomAlpha/GBufferCustomAlpha.h>


GBufferCustomAlpha::GBufferCustomAlpha() : GBufferAbstract(1) {

}

bool GBufferCustomAlpha::init(unsigned int windowW, unsigned int windowH)
{
	startInit();

	CreateFBTexFloat2(windowW, windowH, 0);

	CreateDepthTex(windowW, windowH);

	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufs);

	checkInitStatus();
	return true;
	
}
