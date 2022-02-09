#pragma once

#include "GBuffer.h"

class GBufferAlpha : public GBuffer
{
public:
	enum GBUFFER_ALPHA_TEX_TYPE {
		GBUFFER_ALPHA_TEX_TYPE_ACC_RGBA,
		GBUFFER_ALPHA_TEX_TYPE_BCK,
		GBUFFER_ALPHA_TEX_TYPE_DELTA,
		GBUFFER_ALPHA_TEX_TYPE_NUM_TEXTURES
	};

	GBufferAlpha();
	~GBufferAlpha();
	void bindForWriting();
	//TODO Make abstarct class for GBuffer
	bool init(unsigned int windowW, unsigned int windowH);
};

