#pragma once

#include <ApplicationAbstract.h>
#include <Quad2D.h>
#include <Scene/Scene.h>

class StachosticTransparencyApplication : public ApplicationAbstract {
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
	GLuint mFbo;
	GLuint mDepthTexture, mTexture, mTextureAfter;
	GLuint mFboAlpha;
	GLuint mFboAfter;
	GLuint mAlphaAcc;

	void InitScene();
	void InitShaderAndBuffers();
	void InitFB(unsigned short width, unsigned short height);

	void Quad2DRender(const std::string& shaderName);
public:
	void Render();
	int Init();

	void RenderFrame();

	std::string GetName();

	StachosticTransparencyApplication();
	TestScene* getScene();
};