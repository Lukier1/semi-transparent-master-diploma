#pragma once

#include "../../stdafx.h"

#include "GBufferAbstract.h"

class GBufferArray : public GBufferAbstract {
public:
	GBufferArray();
	bool init(unsigned int windowW, unsigned int windowH);
	//void init(unsigned int width, unsigned int height);
	//void bindForWriting();
	//void bindForReading();
	//void bindForReading(unsigned int offset);
private:
	//GLuint mArrayBuffer;
	//GLuint mFbo;
};