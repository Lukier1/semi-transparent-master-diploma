#pragma once

#include "../stdafx.h"
#include "GBufferAbstract.h"

class GBufferCustomAlpha : public GBufferAbstract {
public:
	GBufferCustomAlpha();
	bool init(unsigned int windowW, unsigned int windowH);
};