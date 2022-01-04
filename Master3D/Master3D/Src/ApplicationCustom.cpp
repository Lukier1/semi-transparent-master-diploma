#include "stdafx.h"

/*
#include <WindowGL.h>
#include "Math\Vec.h"
#include <Viewport.h>
#include <SkyBox.h>
#include <ShaderManager.h>
#include <Scene/Scene.h>
#include <Scene/LightManager.h>
#include <Quad2D.h>
#include <GBuffer.h>
#include <TimeMeasure.h>
#include <ControllerRec.h>
#include <Scene/LightManagerSimpleImpl.h>
#include<CustomAlpha/GBufferArray.h>
//#include <ApplicationCustom.h>
 
/*
GLuint tex2;


void creatTestTex2() {
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);

	int width = 1280, height = 720;
	std::vector<GLfloat> image(4 * width * height);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			size_t index = j * width + i;

			image[4 * index + 0] = std::max(std::min(1.0f, (float)(rand() % 1000) / 1000.0f), 0.0f);
			image[4 * index + 1] = 0.0f;
			image[4 * index + 2] = 0.0f;
			image[4 * index + 3] =  std::max(std::min(0.0f, rand()%1000/1000.0f), 1.0f);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI,
		width, height, 0,
		GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
}

void ApplicationCustom::RenderCustomAlpha()
{
	gBufferCustomAlpha.bindForWriting();
	glDepthMask(true);

	glClearColor(-1000.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_MAX);

	CalcMaxMin();
	
	gBufferCustomAlpha.bindForReading(10);
	gBufferArray.bindForWriting();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_AL, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	MultilayerRender();


	gBufferArray.bindForReading(1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	window.startFrame();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Quad2DRender();
}


int ApplicationCustom::InitGL()
{
	if (window.initWindow() != 0)
		return -1;

	if (GLEW_KHR_debug) {
		int param = -1;
		printf("KHR_debug extension found\n");
		glDebugMessageCallback(debug_gl_callback, &param);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		printf("debug callback engaged\n");
	}
	else {
		printf("KHR_debug extension NOT found\n");
	}

	 get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	###############################
	CONFIG OPENGL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	 During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);

	return 0;
}

void ApplicationCustom::InitScene()
{
	mViewport.setViewSize(Vector2f(window.getScreenWidth(), window.getScreenHeigth()));
	mViewport.setPersp(1.0f, 60.0f);

	##################################
	SCENE OBJECTS 

	mSkyBox = std::unique_ptr<SkyBox>(new SkyBox());
	mSkyBox->Init();

	mScreenQuad = std::unique_ptr<Quad2D>(new Quad2D);
	mScreenQuad->Init();

	mLightManager = std::unique_ptr<LightManager>(new LightManagerSimpleImpl());

	TODO No light bug
	mLightManager->addLight(DirectionLight()
		.SetDirection(Vector3f(0.0f, -1.0f, -1.0f).normalizeVec3())
		.SetDiffusionColor(Vector3f(1.0, 1.0f, 1.0f))
		.SetReflectionColor(Vector3f(1.0f, 1.0f, 1.0f)));


	testScene.loadScene();
}

void ApplicationCustom::InitShaderAndBuffers()
{
	gBuffer.init(window.getScreenWidth(), window.getScreenHeigth());
	
	mShaderManager.putShader("geomPassProg",
		ShaderBuilder()
		.AddShader("Shaders/deffered/GeomPassFragm.glsl", SHADER_TYPE::FRAGMENT)
		.AddShader("Shaders/deffered/GeomPassVert.glsl", SHADER_TYPE::VERTEX)
		.Build());


	mShaderManager.putShader("calcMaxMinProg",
		ShaderBuilder()
		.AddShader("Shaders/CustomAlpha/CalcMaxMinFrag.glsl", SHADER_TYPE::FRAGMENT)
		.AddShader("Shaders/CustomAlpha/CalcMaxMinVert.glsl", SHADER_TYPE::VERTEX)
		.Build());

	mShaderManager.putShader("quad2dProg",
		ShaderBuilder()
		.AddShader("Shaders/CustomAlpha/quad2dFrag.glsl", SHADER_TYPE::FRAGMENT)
		.AddShader("Shaders/CustomAlpha/quad2dVert.glsl", SHADER_TYPE::VERTEX)
		.Build());

	mShaderManager.putShader("calcLightProg",
		ShaderBuilder()
		.AddShader("Shaders/skybox_vert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/deffered/LightCalcFragm.glsl", SHADER_TYPE::FRAGMENT)
		.Build());

	mShaderManager.putShader("multlayerProg",
		ShaderBuilder()
		.AddShader("Shaders/CustomAlpha/multiLayerOutVert.glsl", SHADER_TYPE::VERTEX)
		.AddShader("Shaders/CustomAlpha/multiLayerOutFrag.glsl", SHADER_TYPE::FRAGMENT)
		.Build());


	InitFB(window.getScreenWidth(), window.getScreenHeigth());

	creatTestTex2();

}

void ApplicationCustom::InitFB(unsigned short width, unsigned short height)
{
	gBufferCustomAlpha.init(width, height);
	gBufferArray.init(width, height);

	glGenFramebuffers(1, &mFB);

	glGenTextures(1, &mFBTex);
	glBindTexture(GL_TEXTURE_2D, mFBTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		width, height, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, mFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFBTex, 0);

	glGenTextures(1, &mFBDepthTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFBDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mFBDepthTex, 0);

	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT };
	glDrawBuffers(1, draw_bufs);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		Logger::getLogger()->Error("Incomplete framebuffer\n");
	}
	else
	{
		Logger::getLogger()->Info("Succesfully init of framebuffers\n");
	}


}

void ApplicationCustom::Logic()
{
	Matrix4x4 view_mat = Matrix4x4::viewMatrix(cam.rotation, cam.position);

	const float  ROTATION_SPEED = 0.005f; // Przesuniecie o jeden piksel na klatke myszka przeklada sie na ta ilosc obrotow w ciagu sekundy
	const float MOVE_SPEED = 10.0f; // Predkos w jednostkach na sekunde
	const ControllerRec& rec = ControllerRec::GetSingleton();

	if (rec.IsPushed(GLFW_KEY_C)) {
		NextCameraPos();
	}

	float v[] = { 0,0,0,0 };
	if (rec.IsDown(GLFW_KEY_S))
	{
		v[2] = -MOVE_SPEED;
	}
	if (rec.IsDown(GLFW_KEY_W))
	{
		v[2] = MOVE_SPEED;
	}
	if (rec.IsDown(GLFW_KEY_A))
	{
		v[0] = MOVE_SPEED;
	}
	if (rec.IsDown(GLFW_KEY_D))
	{
		v[0] = -MOVE_SPEED;
	}

	Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2]) * Vec4(v);
	Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]) * (float)mFrameElaps;
	cam.position = cam.position + move3;

	if (cameraMovement) {
		cam.rotation.v[1] += -(float)rec.GetMoveX() * 2.0f * PI * (float)(mFrameElaps)*ROTATION_SPEED;
		cam.rotation.v[0] += (float)rec.GetMoveY() * 2.0f * PI * (float)(mFrameElaps)*ROTATION_SPEED;
	}

	cameraMovement = rec.IsDown(GLFW_KEY_LEFT_ALT);

	mViewport.updateCamera(cam);
}

int ApplicationCustom::Run()
{
	int result;
	if ((result = InitGL()) < 0)
		return result;

	InitScene();
	InitShaderAndBuffers();

	mTimeMeas.reset();

	double rotLight = 0.0;

	while (!window.isClose()) {
		TimeMeasure oneFrame = TimeMeasure::start();

		TimeMeasure checkFrame = TimeMeasure::start();
		RenderDeffered();

		window.finishFrame();

		glfwPollEvents();
		Vec4 testpoint = mViewport.getViewMat() * mViewport.getPerspMat() * Vector4f(0, 0, 0, 1);
		const Vec2 screenCenter = Vector2f(window.getScreenWidth() / 2.0f, window.getScreenHeigth() / 2.0f);

		ControllerRec::GetSingleton().CalculateMove(window.getWindow(), screenCenter);
		mFrameElaps = mTimeMeas.reset();
		Logic();

		ControllerRec::GetSingleton().CleanStates(window.getWindow(), screenCenter);

		mShaderManager.scan(mFrameElaps);
		if (cameraMovement) {
			glfwSetCursorPos(window.getWindow(), screenCenter.v[0], screenCenter.v[1]);
		}
		int sleepTime = (int)(1000.0f * (1 / 60.0f - oneFrame.measure()));
		if (sleepTime > 0) {
			Sleep(sleepTime);
		}

		rotLight += 2 * 3.14 / 120.0;
		DirectionLight* dir = &mLightManager->getDirectionLightList()[0];
	}

	glfwTerminate();
	return 0;
}

void ApplicationCustom::CalcMaxMin() {
	const ShaderProgram* shader = mShaderManager.getShader("calcMaxMinProg");
	shader->UseProgram();

	testScene.drawTransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());
}


void ApplicationCustom::MultilayerRender() {
	const ShaderProgram* shader = mShaderManager.getShader("multlayerProg");
	shader->UseProgram();
	applyLights(shader);
	
	glBindImageTexture(8, tex2, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
		glBindImageTexture(8, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	testScene.drawTransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());
	mScreenQuad->RenderVertices();
}


void ApplicationCustom::Quad2DRender() {
	const ShaderProgram* shader = mShaderManager.getShader("quad2dProg");
	shader->UseProgram();
	mScreenQuad->RenderVertices();
}



void ApplicationCustom::applyLights(const ShaderProgram* program) {
	const auto pointLightList = mLightManager->getPointLightList();
	const auto lightDirList = mLightManager->getDirectionLightList();

	program->apply("lightNumber", pointLightList.size());
	if (pointLightList.size() != 0) {
		std::vector<Vec3> positions;
		std::vector<Vec3> colors;
		std::vector<float> powers;

		for (PointLight light : pointLightList) {
			positions.push_back(light.getPosition());
			colors.push_back(light.GetDiffusionColor());
			powers.push_back(light.GetPower());
		}

		program->apply("lightsPositions", &positions[0], positions.size());
		program->apply("lightsColors", &colors[0], colors.size());
		program->apply("lightsPowers", &powers[0], powers.size());
	}

	program->apply("dirLightNumber", lightDirList.size());
	if (lightDirList.size() != 0) {
		std::vector<Vec3> dirs;
		std::vector<Vec3> colors;

		for (DirectionLight light : lightDirList) {
			dirs.push_back(light.GetDirection());
			colors.push_back(light.GetDiffusionColor());
		}
		if (lightDirList.size() != 0) {
			program->apply("dirLightDirection", &dirs[0], dirs.size());
			program->apply("dirLightColor", &colors[0], colors.size());
		}
	}

	program->apply("ambientLight", ambientLight);

	program->apply("cameraPosition", cam.position);
}

void ApplicationCustom::LightPass() {
	const ShaderProgram* calcLightProg = mShaderManager.getShader("calcLightProg");

	calcLightProg->UseProgram();

	applyLights(calcLightProg);
	calcLightProg->apply("cameraPosition", cam.position);
	calcLightProg->apply("VPShadowMap", vpShadowMatrix());
	calcLightProg->apply("screen", Vector2f(window.getScreenWidth(), window.getScreenHeigth()));

	gSingleTexBuffer.bindForReading(5);


	calcLightProg->apply("skyBoxTexture", mSkyBox->mTexture.getTexID());
	calcLightProg->applyTexture(6, mSkyBox->mTexture.getTexID(), true);
	calcLightProg->apply("P", mViewport.getPerspMat());
	calcLightProg->apply("V", mViewport.getViewMat());

	mSkyBox->Render2(mViewport);

	glDepthMask(GL_TRUE);
}

Matrix4x4 ApplicationCustom::vpShadowMatrix() {
	Vec3 invLight = mLightManager->getDirectionLightList()[0].GetDirection() * (-1);

	const Matrix4x4 perspMat = Matrix4x4::ortographic(Vector3f(-50, -20, 50), Vector3f(50, 50, -50));
	const Matrix4x4 cameraMat = Matrix4x4::lookat(invLight, Vector3f(0, 0, 0));
	Matrix4x4 vpMat = cameraMat * perspMat;

	float v[16] = { 0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0.5f, 0.5f, 0.5f, 1.0f };
	return vpMat * Matrix4x4(v);
}


void ApplicationCustom::RenderDeffered()
{
	TimeMeasure alphaPass = TimeMeasure::start();

	gBuffer.bindForWriting();
	glDepthMask(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ObscuredPassGeom();

	glViewport(0, 0, 1280, 720);

	gBuffer.bindForReading();
	glBindFramebuffer(GL_FRAMEBUFFER, mFB);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(true);



	ALPHA PART
	gBufferCustomAlpha.bindForWriting();
	glDepthMask(true);

	glClearColor(-1000.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_MAX);

	CalcMaxMin();

	gBufferCustomAlpha.bindForReading(10);
	gBufferArray.bindForWriting();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_AL, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	MultilayerRender();


	gBufferArray.bindForReading(1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	window.startFrame();
	LightPass();
	glClearColor(1, 1, 1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, mFBTex);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	Quad2DRender();


}

void ApplicationCustom::ObscuredPassGeom() {
	const ShaderProgram* shader = mShaderManager.getShader("geomPassProg");
	shader->UseProgram();

	testScene.drawIntransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());
}

void ApplicationCustom::NextCameraPos() {
	static int ind = 0;
	const float camPos[4][2][3] = {
		{{10.104733, 7.106497, 3.431073},{-0.001761, -1.534323, 0.000000}},
			{{7.063844, 13.699590, 1.455959},{0.654297, -1.373446, 0.000000}},
			{{8.618509, 1.351903, 3.387586},{-0.617451, -1.606352, 0.000000}},
			{{8.618509, 1.351903, 3.387586},{-0.617451, -1.606352, 0.000000}}
	};
	ind = (ind+1) % 4;
	cam.position = Vector3f(camPos[ind][0][0], camPos[ind][0][1], camPos[ind][0][2]);
	cam.rotation = Vector3f(camPos[ind][1][0], camPos[ind][1][1], camPos[ind][1][2]);
} */