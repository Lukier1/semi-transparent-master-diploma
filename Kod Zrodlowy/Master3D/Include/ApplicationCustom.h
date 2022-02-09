#pragma once

#include "../stdafx.h"

/*
#include <CustomAlpha/GBufferCustomAlpha.h>
#include <CustomAlpha/GBufferArray.h>

#pragma once
class ApplicationCustom {
private:
	WindowGL window;

	//##### TEST
	Vec3 lightDir = Vector3f(0.01, -1, 0);

	//#################
	//SCENE OBJECTS

	Viewport mViewport;
	Camera cam;
	std::unique_ptr<SkyBox> mSkyBox;
	std::unique_ptr<Quad2D> mScreenQuad;

	std::unique_ptr<LightManager> mLightManager;

	TestScene testScene;
	//SCENE Parameters
	const Vec3 ambientLight = Vector3f(0.3f, 0.3f, 0.3f);
	const Vec3 bigSphereColor = Vector3f(0.6f, 0.0f, 0.0f);
	const Vec3 smallSphereColor = Vector3f(0.0f, 0.6f, 0.0f);
	const Vec3 glassColor = Vector3f(0.0f, 0.0f, 0.6f);

	//DEFFERED SHADING
	GBuffer gBuffer;
	
	//Custom Alpha
	GBufferCustomAlpha gBufferCustomAlpha;
	GBufferArray gBufferArray;

	//################
	TimeMeasure mTimeMeas;
	double mFrameElaps = 0.0f;

	//Scene framebuffer 
	GLuint mFB, mFBTex, mFBDepthTex;

	//###################
	ShaderManager mShaderManager;


	bool cameraMovement = false;

	void InitScene();
	void InitShaderAndBuffers();
	void InitFB(unsigned short width, unsigned short height);

	void RenderDeffered();
	void ObscuredPassGeom();
	void NextCameraPos();
	void Logic();

	void RenderCustomAlpha();

	int InitGL();
	
	void CalcMaxMin();

	void MultilayerRender();

	void Quad2DRender();

	void applyLights(const ShaderProgram* program);

	void LightPass();

public:
	int Run();

	Matrix4x4 vpShadowMatrix();

	ApplicationCustom() : cam(Vector3f(0, 5.0f, 10.0f), Vector3f(0.4f, -PI, 0)),
		mViewport(Vector2f(800, 600), cam), mTimeMeas(TimeMeasure::start()) {
	}
};

*/