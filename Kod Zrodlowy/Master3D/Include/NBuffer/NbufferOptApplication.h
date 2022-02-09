#pragma once

#include "../stdafx.h"

#include <CustomAlpha/GBufferCustomAlpha.h>
#include <CustomAlpha/GBufferArray.h>
#include <ApplicationAbstract.h>


class NbufferOptApplication : public ApplicationAbstract {
private:
	//#################
	//SCENE OBJECTS
	std::unique_ptr<Quad2D> mScreenQuad;

	TestScene testScene;

	//Abuffer Tex Buffers
	GLuint texCounter;
	GLuint texBuffer;
	GLuint alphaBuffer;

	//Test buffer
	GLuint lockBuffer;

	

	//Scene framebuffer 
	GLuint mFB, mFBTex, mFBDepthTex;
	
	GLuint mOutputDepthFBO[8], mOutputDepthTex[8], mOutputColorTex[8];
	void InitScene();
	void InitShaderAndBuffers();
	void InitFB(unsigned short width, unsigned short height);

	void Quad2DRender(const std::string &shaderName);



public:
	void Render();
	int Init();

	void RenderFrame();

	std::string GetName();

	NbufferOptApplication();

	TestScene* getScene();
};

