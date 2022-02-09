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
#include <NBuffer/NbufferOptApplication.h>


const int ABUFFER_SIZE_M = 32;
const int NBUFFER_SIZE_M = 4;

int NbufferOptApplication::Init() {
	int result;
	if ((result = InitGL()) < 0)
		return result;

	InitScene();
	InitShaderAndBuffers();

	return 0;
}

void NbufferOptApplication::InitScene()
{
	//##################################
	//SCENE OBJECTS 

	mScreenQuad = std::unique_ptr<Quad2D>(new Quad2D);
	mScreenQuad->Init();

	testScene.loadScene();
}

void NbufferOptApplication::InitShaderAndBuffers()
{
	mShaderManager.putShader("nBufferRenderObject",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/NBufferRenderObjectVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/NBufferDepthFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());


	mShaderManager.putShader("nBufferFinishRender",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/NBufferFinishRenderVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/NBufferFinishRenderFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("nBufferClean",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/NBufferFinishRenderVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/NBufferCleanFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("depthPeeling",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/DepthPeelingVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/DepthPeelingFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());


	mShaderManager.putShader("farestComposite",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/DepthPeelingVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/CompositeFarestFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("testBuffer",
		ShaderBuilder()
		.AddShader("Shaders/NBufferOpt/NBufferFinishRenderVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/NBufferOpt/TestFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	InitFB(window.getScreenWidth(), window.getScreenHeigth());

}

void NbufferOptApplication::InitFB(unsigned short width, unsigned short height)
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

	glGenFramebuffers(8, mOutputDepthFBO);
	glGenTextures(8, mOutputDepthTex);
	glGenTextures(8, mOutputColorTex);

	for (int i = 0; i < 3; i++) {
		
		glBindTexture(GL_TEXTURE_2D, mOutputDepthTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mOutputDepthFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mOutputDepthTex[i], 0);

		glBindTexture(GL_TEXTURE_2D, mOutputColorTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mOutputColorTex[i], 0);

		GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, draw_bufs);

	}

}


std::string NbufferOptApplication::GetName()
{
	return "NBufferOpt";
}


void NbufferOptApplication::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_MULTISAMPLE);
	//Disable stencil test
	//glDisable(GL_STENCIL_TEST);
	//Disable backface culling to keep all fragments
	glDisable(GL_CULL_FACE);

	//ALPHA PART
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	//Clean depth in frame buffer 0 
	glBindFramebuffer(GL_FRAMEBUFFER, mOutputDepthFBO[0]);
	glClearDepth(0);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 3; ++i) {
		glClearDepth(1);
		if (i != 0) {
			glBindTexture(GL_TEXTURE_2D, mOutputDepthTex[i - 1]);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, mOutputDepthTex[0]);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, mOutputDepthFBO[i]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const ShaderProgram* shader = mShaderManager.getShader("depthPeeling");
	
		shader->UseProgram();
		testScene.applyLights(shader);
		if (i == 0) {
			shader->apply("stage", 0);
		}
		else {
			shader->apply("stage", 1);
		}
		shader->apply("cameraPosition", cam.position);
		testScene.drawTransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());
		
	}
	//Test Part
	const ShaderProgram* testBufferShader = mShaderManager.getShader("testBuffer");

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	window.startFrame();
	glClearColor(1, 1, 1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const ShaderProgram* farestComposite = mShaderManager.getShader("farestComposite");
	const ShaderProgram* usedShader = farestComposite;
	usedShader->UseProgram();
	usedShader->applyTexture(GL_TEXTURE4, mOutputColorTex[0], false);
	usedShader->applyTexture(GL_TEXTURE5, mOutputColorTex[1], false);
	usedShader->applyTexture(GL_TEXTURE6, mOutputColorTex[2], false);

	
	usedShader->applyTexture(GL_TEXTURE7, mOutputDepthTex[0], false);
	usedShader->applyTexture(GL_TEXTURE8, mOutputDepthTex[1], false);
	usedShader->applyTexture(GL_TEXTURE9, mOutputDepthTex[2], false);


	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	testScene.applyLights(usedShader);
	testScene.drawTransparentObjects(usedShader, mViewport.getPerspMat(), mViewport.getViewMat());
	for (int i = 2; i >= 0; --i) {
		usedShader = mShaderManager.getShader("testBuffer");
		usedShader->applyTexture(GL_TEXTURE4, mOutputColorTex[i], false);
		usedShader->apply("cameraPosition", cam.position);
		Quad2DRender("testBuffer");
	}
}


void NbufferOptApplication::RenderFrame() {
	Render();
}

NbufferOptApplication::NbufferOptApplication() {
}


void NbufferOptApplication::Quad2DRender(const std::string& shaderName) {
	const ShaderProgram* shader = mShaderManager.getShader(shaderName);
	shader->UseProgram();
	shader->apply("BUFFER_SIZE", NBUFFER_SIZE_M);
	mScreenQuad->RenderVertices();
}

TestScene * NbufferOptApplication::getScene() {
	return &testScene;
}