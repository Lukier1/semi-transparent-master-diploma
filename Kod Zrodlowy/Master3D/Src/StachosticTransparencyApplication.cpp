#include "stdafx.h"


#include <StachosticTransparencyApplication.h>

void StachosticTransparencyApplication::InitScene()
{
	mScreenQuad = std::unique_ptr<Quad2D>(new Quad2D);
	mScreenQuad->Init();

	testScene.loadScene();
}

void StachosticTransparencyApplication::InitShaderAndBuffers()
{

	mShaderManager.putShader("stochasticAlpha",
		ShaderBuilder()
		.AddShader("Shaders/Stochastic/StochasticAlphaVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/Stochastic/StochasticAlphaFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());


	mShaderManager.putShader("stochasticFinish",
		ShaderBuilder()
		.AddShader("Shaders/Stochastic/StochasticFinishVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/Stochastic/StochasticFinishFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("stochasticSampling",
		ShaderBuilder()
		.AddShader("Shaders/Stochastic/StochasticSamplingVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/Stochastic/StochasticSamplingFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("mergeWithBck",
		ShaderBuilder()
		.AddShader("Shaders/Stochastic/StochasticMegeFinishFrag.glsl", SHADER_TYPE::FRAGMENT)
		.AddShader("Shaders/quad2d_vert.glsl", SHADER_TYPE::VERTEX)
		.Build());

	InitFB(window.getScreenWidth(), window.getScreenHeigth());
}

void StachosticTransparencyApplication::InitFB(unsigned short width, unsigned short height)
{

	glGenFramebuffers(1, &mFboAfter);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFboAfter);

	glGenTextures(1, &mTextureAfter);

	glBindTexture(GL_TEXTURE_2D, mTextureAfter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		width, height, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureAfter, 0);

	glGenFramebuffers(1, &mFboAlpha);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFboAlpha);

	glGenTextures(1, &mAlphaAcc);
	glBindTexture(GL_TEXTURE_2D, mAlphaAcc);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
	glBindImageTexture(0, mAlphaAcc, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAlphaAcc, 0);

	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mTexture);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA16F,
		width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture, 0);

	glGenTextures(1, &mDepthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mDepthTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_DEPTH_COMPONENT32,
		width, height, GL_TRUE);

	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of shadow framebuffer\n");
	}
}

void StachosticTransparencyApplication::Quad2DRender(const std::string& shaderName)
{
	const ShaderProgram* shader = mShaderManager.getShader(shaderName);
	shader->UseProgram();
	mScreenQuad->RenderVertices();
}

void StachosticTransparencyApplication::Render()
{
	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFboAlpha);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	const ShaderProgram* alphaShader = mShaderManager.getShader("stochasticAlpha");

	alphaShader->UseProgram();
	testScene.drawTransparentObjects(alphaShader, mViewport.getPerspMat(), mViewport.getViewMat());
	
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat ACC_DEPTH_CLEAR[] = { 1 };

	glClearBufferfi(GL_DEPTH, mDepthTexture, 1, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_BLEND);
	glDisable(GL_SAMPLE_SHADING);
	glMinSampleShading(1.0f);
	glDepthMask(true);
	const ShaderProgram * samplingShader = mShaderManager.getShader("stochasticSampling");
	
	samplingShader->UseProgram();
	samplingShader->apply("cameraPosition", cam.position);
	testScene.applyLights(samplingShader);
	testScene.drawTransparentObjects(samplingShader, mViewport.getPerspMat(), mViewport.getViewMat());


	//glBindFramebuffer(GL_FRAMEBUFFER, mFboAfter);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	window.startFrame();
	glClearColor(1, 1, 1, 0); 
	//glClearColor(0, 0, 0, 0); // For depth vis
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST); // For depth vis
	//glBlendEquation(GL_FUNC_ADD); // For depth vis
	//glBlendFunc(GL_ONE, GL_ONE); // For depth vis

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const ShaderProgram* finishShader = mShaderManager.getShader("stochasticFinish");
	finishShader->UseProgram();


	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mDepthTexture);
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, mAlphaAcc);
	
	finishShader->apply("cameraPosition", cam.position);
	testScene.applyLights(finishShader);
	testScene.drawTransparentObjects(finishShader, mViewport.getPerspMat(), mViewport.getViewMat());
	
	/*glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, mAlphaAcc);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mTextureAfter);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const ShaderProgram* mergeShader = mShaderManager.getShader("mergeWithBck");

	mergeShader->UseProgram();
	mScreenQuad->RenderVertices();*/
}

int StachosticTransparencyApplication::Init()
{
	int result;
	if ((result = InitGL()) < 0)
		return result;

	InitScene();
	InitShaderAndBuffers();

	return 0;
}

void StachosticTransparencyApplication::RenderFrame()
{
	Render();
}

std::string StachosticTransparencyApplication::GetName()
{
	return "stochastic";
}

StachosticTransparencyApplication::StachosticTransparencyApplication() {

}

TestScene* StachosticTransparencyApplication::getScene() {
	return &testScene;
}