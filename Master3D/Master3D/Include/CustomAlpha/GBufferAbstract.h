#pragma once

#include "../stdafx.h"


/*
* Kod z http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html
*/
class GBufferAbstract {
public:
	virtual ~GBufferAbstract();

	virtual bool init(unsigned int windowW, unsigned int windowH) = 0;

	void bindForWriting();
	void bindForReading();

	void bindForReading(unsigned int offset);

	GLuint getTexture(int bufferIndex);

	GLuint getDepthTexture();

protected:
	GBufferAbstract(const int numberOfBuffer);

	GLuint mFbo;
	GLuint * mTextures;
	GLuint mDepthTexture = -1;

	const unsigned int GBUFFER_NUM;

	void CreateFBTex(unsigned int windowW, unsigned int windowH, int num);
	void CreateFBTexFloat(unsigned int windowW, unsigned int windowH, int num);
	void CreateFBTexFloat2(unsigned int windowW, unsigned int windowH, int num);
	void CreateDepthTex(unsigned int windowW, unsigned int windowH);

	void startInit();
	void checkInitStatus();
};