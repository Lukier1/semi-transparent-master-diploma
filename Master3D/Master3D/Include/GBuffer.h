#pragma once

#include "../stdafx.h"


/*
* Kod z http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html
*/
class GBuffer {
public:
	enum GBUFFER_TEX_TYPE {
		GBUFFER_TEX_TYPE_POSITION,
		GBUFFER_TEX_TYPE_DIFFUSE,
		GBUFFER_TEX_TYPE_NORMAL,
		GBUFFER_TEX_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	virtual ~GBuffer();

	bool init(unsigned int windowW, unsigned int windowH);

	void CreateFBTexFloat(unsigned int windowW, unsigned int windowH, int num);

	void bindForWriting();
	void bindForReading();

	void bindForReading(unsigned int offset);
	
	GLuint getTexture(GBUFFER_TEX_TYPE bufferTexType);
	GLuint getDepthTexture();

protected:
	GBuffer(const int numberOfBuffer);

	GLuint mFbo;
	GLuint * mTextures;
	GLuint mDepthTexture;

	const unsigned int GBUFFER_NUM;

	void CreateFBTex(unsigned int windowW, unsigned int windowH, int num);


};