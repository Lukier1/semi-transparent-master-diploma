#include "stdafx.h"


#include <WindowGL.h>
#include "Math\Vec.h"
#include <Viewport.h>
#include <ShaderManager.h>
#include <Scene/Scene.h>
#include <Scene/LightManager.h>
#include <Quad2D.h>
#include <GBuffer.h>
#include <TimeMeasure.h>
#include <ControllerRec.h>
#include <Scene/LightManagerSimpleImpl.h>
#include <CustomAlpha/GBufferArray.h>
#include <NBuffer/NbufferApplication.h>


const int ABUFFER_SIZE_M = 32;
const int NBUFFER_SIZE_M = 4;

int NbufferApplication::Init() {
	int result;
	if ((result = InitGL()) < 0)
		return result;

	InitScene();
	InitShaderAndBuffers();

	return 0;
}

void NbufferApplication::InitScene()
{
	//##################################
	//SCENE OBJECTS 

	mScreenQuad = std::unique_ptr<Quad2D>(new Quad2D);
	mScreenQuad->Init();

	testScene.loadScene();
}

void NbufferApplication::InitShaderAndBuffers()
{
	mShaderManager.putShader("nBufferRenderObject",
		ShaderBuilder()
		.AddShader("Shaders/NBuffer/NBufferRenderObjectVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBuffer/NBufferRenderObjectFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());


	mShaderManager.putShader("nBufferFinishRender",
		ShaderBuilder()
		.AddShader("Shaders/NBuffer/NBufferFinishRenderVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBuffer/NBufferFinishRenderFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("nBufferClean",
		ShaderBuilder()
		.AddShader("Shaders/NBuffer/NBufferFinishRenderVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBuffer/NBufferCleanFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	InitFB(window.getScreenWidth(), window.getScreenHeigth());

}

void NbufferApplication::InitFB(unsigned short width, unsigned short height)
{
	glGenTextures(1, &lockBuffer);
	glBindTexture(GL_TEXTURE_2D, lockBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI,
		width, height, 0,
		GL_RED_INTEGER, GL_INT, NULL);

	glBindImageTexture(2, lockBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32I);
		
	
	glGenTextures(1, &texCounter);
	glBindTexture(GL_TEXTURE_2D, texCounter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI,
		width, height, 0,
		GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	glBindImageTexture(0, texCounter, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

	glGenTextures(1, &texBuffer);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, width, height, ABUFFER_SIZE_M);
	glBindImageTexture(1, texBuffer, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

	glGenTextures(1, &alphaBuffer);
	glBindTexture(GL_TEXTURE_2D_ARRAY, alphaBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R32F, width, height, ABUFFER_SIZE_M);
	glBindImageTexture(3, alphaBuffer, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);
}


std::string NbufferApplication::GetName()
{
	return "NBuffer";
}


void NbufferApplication::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	//Disable stencil test
	glDisable(GL_STENCIL_TEST);
	//Disable backface culling to keep all fragments
	glDisable(GL_CULL_FACE);
	glDepthMask(false);

	//ALPHA PART
	//
	//Clear Buffer

	Quad2DRender("nBufferClean");
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glClearColor(1, 1, 1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Fill A-Buffer
	
	const ShaderProgram* shader = mShaderManager.getShader("nBufferRenderObject");
	shader->UseProgram();
	shader->apply("BUFFER_SIZE", NBUFFER_SIZE_M);
	shader->apply("cameraPosition", cam.position);
	testScene.applyLights(shader);
	testScene.drawTransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());


	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glBindImageTexture(0, texCounter, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);

	//Sort A-Buffer and render result
	window.startFrame();
	
	glClearColor(1, 1, 1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	Quad2DRender("nBufferFinishRender");
}


void NbufferApplication::RenderFrame() {
	Render();
}

NbufferApplication::NbufferApplication() {
}


void NbufferApplication::Quad2DRender(const std::string& shaderName) {
	const ShaderProgram* shader = mShaderManager.getShader(shaderName);
	shader->UseProgram();
	shader->apply("BUFFER_SIZE", NBUFFER_SIZE_M);
	mScreenQuad->RenderVertices();
}

TestScene * NbufferApplication::getScene() {
	return &testScene;
}