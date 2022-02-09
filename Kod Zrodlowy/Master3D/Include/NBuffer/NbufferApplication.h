#pragma once

#include "../stdafx.h"

#include <CustomAlpha/GBufferCustomAlpha.h>
#include <CustomAlpha/GBufferArray.h>
#include <ApplicationAbstract.h>


class NbufferApplication : public ApplicationAbstract {
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


	void InitScene();
	void InitShaderAndBuffers();
	void InitFB(unsigned short width, unsigned short height);

	void Quad2DRender(const std::string &shaderName);



public:
	void Render();
	int Init();

	void RenderFrame();

	std::string GetName();

	NbufferApplication();

	TestScene* getScene();
};

