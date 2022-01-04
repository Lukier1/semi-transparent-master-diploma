#pragma once

#include "../stdafx.h"
#include "Math\Vec.h"
class StachosticShadowBuffer {
public:
	StachosticShadowBuffer();
	~StachosticShadowBuffer();

	void bindForWriting();
	void bindForReading();

	void bindForReading(unsigned int offset);

	GLuint getDepthTexture();
	GLuint getTexture();
	//TODO Make abstarct class for GBuffer
	bool init(unsigned int res);

	GLuint getFBO() const;
private:
	GLuint mFbo;

	GLuint mDepthTexture, mTexture, mOneSampleTexture;
};

class MultiSampleBuffer {
public:
	MultiSampleBuffer();
	~MultiSampleBuffer();

	void bindForWriting();
	void bindForReading(unsigned int offset = 0);

	bool init(unsigned int width, unsigned int height);

	GLuint getMSTexture() const;

	GLuint getFBO() const;
private:
	GLuint mFbo;

	GLuint mMultiSampleTex;
	GLuint mMultiSampleDepthTex;
	Vec2 size;
};

class SingleTexBuffer {
public:
	SingleTexBuffer();
	~SingleTexBuffer();

	void bindForWriting();
	void bindForReading(unsigned int offset = 0);

	bool init(unsigned int width, unsigned int height);

	GLuint getTexture();

	GLuint getFBO() const;
private:
	GLuint mFbo;

	GLuint mTexture, mDepthTexture;
};

//TODO: 
//Bufory do filtrowania tekstury
//Bufor laczacy z kilku sampli
//Multisample -> Tex
//Bufor filtrujacy
//Tex -> Tex