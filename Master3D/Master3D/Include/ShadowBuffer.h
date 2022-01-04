#pragma once

#include "../stdafx.h"

class ShadowBuffer {
public:
	ShadowBuffer();
	~ShadowBuffer();

	void bindForWriting();
	void bindForReading();

	void bindForReading(unsigned int offset);

	GLuint getDepthTexture();
	//TODO Make abstarct class for GBuffer
	bool init(unsigned int res);

	GLuint getFBO() const; 
private:
	GLuint mFbo;

	GLuint mDepthTexture;
};