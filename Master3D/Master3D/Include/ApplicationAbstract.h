#pragma once

#include <Resources/ShaderProgram.h>

#include <WindowGL.h>
#include <Viewport.h>
#include <TimeMeasure.h>
#include <ShaderManager.h>
#include <Scene/Scene.h>


class ApplicationAbstract {
protected:
	Camera cam;
	Viewport mViewport;
	float alpha;
	void ApplyAlpha(const Resources::ShaderProgram* shaderProgram);
	bool cameraMovement = false;

	double mFrameElaps = 0.0f;	
	WindowGL window;

	//################
	TimeMeasure mTimeMeas;


	//###################
	ShaderManager mShaderManager;

public:
	
	ApplicationAbstract();
	virtual ~ApplicationAbstract();
	void RenderFrame();

	virtual std::string GetName() = 0;
	virtual int Init() = 0;
	int Run();
	void doTest();
	virtual void Render() = 0;

	void SetAlpha(float alpha);
	void Logic();
	void NextCameraPos();
	int InitGL();
	WindowGL* GetWindow();
	void SetCamera(Vec3 position, Vec3 rotation);
	virtual TestScene* getScene() = 0;
};