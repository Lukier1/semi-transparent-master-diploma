// CobolGL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"


#include <Exceptions\IOException.h>

#include <CobMath.h>
#include <ControllerRec.h>
#include <WindowGL.h>
#include <SkyBox.h>
#include <Billboard.h>
#include <Quad2D.h>
#include <GLUtils.h>
#include <Viewport.h>
#include <TimeMeasure.h>
#include <PhongShader.h>

#include <ShadowBuffer.h>
#include <StachosticShadowBuffer.h>
#include <GBuffer.h>
#include <GBufferAlpha.h>

#include <ShaderManager.h>

#include <Geometry\Plane.h>

#include <Resources\ShaderProgram.h>
#include <Resources\ModelLoader.h>
#include <Resources\Mesh.h>

#include <Scene\PointLight.h>
#include <Scene\LightManagerSimpleImpl.h>
#include <Scene\ModelObject.h>
#include <Scene\TransparentModelObject.h>

#include <Particles\AlphaParticle.h>

#include <Properties.h>

#include <Scene/Scene.h>
#include <NBuffer/NbufferApplication.h>
#include <ABuffer/AbufferApplication.h>
#include <StachosticTransparencyApplication.h>

using namespace Resources;

int maxFrame = 1000;
int actFrame = 0;
double avgGenShad = 0.0, avgRenGeo = 0.0, avgLightPass = 0.0, avgAlphaPass = 0.0, avgRenderQuad = 0.0, avgDeff = 0.0;

bool cameraMovement = false;


class Application : public ApplicationAbstract {
private:
	//##### TEST
	Vec3 lightDir = Vector3f(0.01, -1, 0);

	//#################
	//SCENE OBJECTS

	std::unique_ptr<SkyBox> mSkyBox;
	std::unique_ptr<Quad2D> mScreenQuad;

	TestScene testScene;
	//SCENE Parameters
	const Vec3 ambientLight = Vector3f(0.3f, 0.3f, 0.3f);
	const Vec3 bigSphereColor = Vector3f(0.6f, 0.0f, 0.0f);
	const Vec3 smallSphereColor = Vector3f(0.0f, 0.6f, 0.0f);
	const Vec3 glassColor = Vector3f(0.0f, 0.0f, 0.6f);

	//DEFFERED SHADING
	GBuffer gBuffer;
	GBufferAlpha gBufferAlpha;
	ShadowBuffer gShadowBuffer;
	StachosticShadowBuffer gStachosticBuffer;
	

	//Alpha Particle
	std::unique_ptr<Particles::AlphaParticle> particle;

	//Shadow filtering
	MultiSampleBuffer gMultisampleBuffer;
	SingleTexBuffer gSingleTexBuffer;

	//Scene framebuffer 
	GLuint mFB, mFBTex, mFBDepthTex;
	

	enum BUFFERENUM {
		DEPTH = 0,
		COLOR = 1,
		END = 2
	} mViewBuffer = BUFFERENUM::COLOR;
	int frameIndex = 0;


	TestScene* getScene() {
		return &testScene;
	}
	//#####################
    //INIT Methods


	int InitGL() {
		if (window.initWindow() != 0)
			return -1;

		if (GLEW_KHR_debug) {
			//int param = -1;
			printf("KHR_debug extension found\n");
			//glDebugMessageCallback(debug_gl_callback, &param);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			printf("debug callback engaged\n");
		}
		else {
			printf("KHR_debug extension NOT found\n");
		}

		// get version info
		const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString(GL_VERSION); // version as a string
		printf("Renderer: %s\n", renderer);
		printf("OpenGL version supported %s\n", version);

		//###############################
		//CONFIG OPENGL
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
		return 0;
	}

	void InitScene() {
		mViewport.setViewSize(Vector2f(window.getScreenWidth(), window.getScreenHeigth()));
		mViewport.setPersp(1.0f, 60.0f);

		//##################################
		//SCENE OBJECTS 
		particle = std::unique_ptr<Particles::AlphaParticle>(new Particles::AlphaParticle(Texture("Data/Images/smoke.png", TextureType::DIFFUSE_TEXTURE)));
		particle->Init();

		mSkyBox = std::unique_ptr<SkyBox>(new SkyBox());
		mSkyBox->Init();

		mScreenQuad = std::unique_ptr<Quad2D>(new Quad2D);
		mScreenQuad->Init();

		testScene.loadScene();
	}

	void InitShaderAndBuffers() {
		gBuffer.init(window.getScreenWidth(), window.getScreenHeigth());
		gBufferAlpha.init(window.getScreenWidth(), window.getScreenHeigth());

		mShaderManager.putShader("geomPassProg",
			ShaderBuilder()
			.AddShader("Shaders/deffered/GeomPassFragm.glsl", SHADER_TYPE::FRAGMENT)
			.AddShader("Shaders/deffered/GeomPassVert.glsl", SHADER_TYPE::VERTEX)
			.Build());

		mShaderManager.putShader("calcLightProg",
			ShaderBuilder()
			.AddShader("Shaders/skybox_vert.glsl", SHADER_TYPE::VERTEX)
			.AddShader("Shaders/deffered/LightCalcFragm.glsl", SHADER_TYPE::FRAGMENT)
			.Build());

		mShaderManager.putShader("alphaPassProg",
			ShaderBuilder()
			.AddShader("Shaders/deffered/AlphaPassFragm.glsl", SHADER_TYPE::FRAGMENT)
			.AddShader("Shaders/deffered/AlphaPassVert.glsl", SHADER_TYPE::VERTEX)
			.Build());


		initFB(window.getScreenWidth(), window.getScreenHeigth());

		gShadowBuffer.init(4096);
		gStachosticBuffer.init(Properties::SHADOW_SIZE);

		gMultisampleBuffer.init(Properties::SHADOW_SIZE, Properties::SHADOW_SIZE);
		gSingleTexBuffer.init(window.getScreenWidth(), window.getScreenHeigth());

		mShaderManager.putShader("shadowPassProg",
			ShaderBuilder()
			.AddShader("Shaders/shadows/ShadowOpaqFragm.glsl", SHADER_TYPE::FRAGMENT)
			.AddShader("Shaders/shadows/ShadowOpaqVert.glsl", SHADER_TYPE::VERTEX)
			.Build());
		mShaderManager.putShader("shadowStachosticPassProg",
			ShaderBuilder()
			.AddShader("Shaders/shadows/ShadowStachosticFragm.glsl", SHADER_TYPE::FRAGMENT)
			.AddShader("Shaders/shadows/ShadowOpaqVert.glsl", SHADER_TYPE::VERTEX)
			.Build());

		mShaderManager.putShader("shadowStachosticMergeFast",
			ShaderBuilder()
			.AddShader("Shaders/shadows/ShadowStachosticMergeFastFragm.glsl", SHADER_TYPE::FRAGMENT)
			.AddShader("Shaders/quad2d_vert.glsl", SHADER_TYPE::VERTEX)
			.Build());

		mShaderManager.putShader("alpaParticleProg",
			ShaderBuilder()
			.AddShader("Shaders/particles/particleVert.glsl", SHADER_TYPE::VERTEX)
			.AddShader("Shaders/particles/particleFrag.glsl", SHADER_TYPE::FRAGMENT)
			.Build());

	}

	void initFB(int width, int height) {
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

	//#####################

	void RenderDeffered()
	{
		TimeMeasure alphaPass = TimeMeasure::start();

		avgGenShad += alphaPass.reset();

		gBuffer.bindForWriting();
		glDepthMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_SAMPLE_SHADING);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//TODO FOR TESTS
		//ObscuredPassGeom();
		GenerateStachosticShadow();


		glViewport(0, 0, 1280, 720);
		avgRenGeo += alphaPass.reset();
		gBuffer.bindForReading();
		glBindFramebuffer(GL_FRAMEBUFFER, mFB);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(true);

		LightPass();

		avgLightPass += alphaPass.reset();
		//glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glDepthMask(true);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getDepthTexture());
		gBufferAlpha.bindForWriting();
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);


		AlphaPassGeom();
		avgAlphaPass += alphaPass.reset();

		glDepthFunc(GL_LESS);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		window.startFrame();

		gBufferAlpha.bindForReading(1);

		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getDepthTexture());

		mScreenQuad->Render(mFBTex, Quad2D::QUAD_SHADERS_BCK_ALPHA, mViewport);
		avgRenderQuad += alphaPass.reset();
	}

	void ObscuredPassGeom() {
		const ShaderProgram * shader = mShaderManager.getShader("geomPassProg");
		shader->UseProgram();

		testScene.drawIntransparentObjects(shader, mViewport.getPerspMat(), mViewport.getViewMat());
	}

	void GenerateStachosticShadow() {
		//Render the opaque objects shadows
		//
		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
		{
			//printf("Error while blitting: %d \n", err);
		}

		glDisable(GL_MULTISAMPLE);
		gShadowBuffer.bindForWriting();
		glDepthMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glViewport(0, 0, Properties::SHADOW_SIZE, Properties::SHADOW_SIZE);
		ShadowMapPass();

		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);

		gStachosticBuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		mScreenQuad->Render(gShadowBuffer.getDepthTexture(), Quad2D::QUAD_SHADERS_TEXTURE_1B, mViewport);

		//--------------------------------------
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_SAMPLE_SHADING);


		glMinSampleShading(1.0f);

		gStachosticBuffer.bindForWriting();
		glDepthMask(true);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_MIN);
		glViewport(0, 0, Properties::SHADOW_SIZE, Properties::SHADOW_SIZE);

		//TODO Switched off
		const Vec3 invLight = Vector3f(1, 0, 0);// mLightManager->getDirectionLightList()[0].GetDirection()* (-1);

		const Matrix4x4 perspMat = Matrix4x4::ortographic(Vector3f(-50, -20, 50), Vector3f(50, 50, -50));
		const Matrix4x4 cameraMat = Matrix4x4::lookat(invLight, Vector3f(0, 0, 0));

		const ShaderProgram* shader = mShaderManager.getShader("shadowStachosticPassProg");
		shader->UseProgram();
		testScene.drawTransparentObjects(shader, perspMat, cameraMat);

		fastStachosticShadowMerge();
	}

	void LightPass() {
		const ShaderProgram* calcLightProg = mShaderManager.getShader("calcLightProg");

		calcLightProg->UseProgram();

		testScene.applyLights(calcLightProg);
		calcLightProg->apply("cameraPosition", cam.position);
		calcLightProg->apply("VPShadowMap", vpShadowMatrix());
		calcLightProg->apply("screen", Vector2f(window.getScreenWidth(), window.getScreenHeigth()));

		gSingleTexBuffer.bindForReading(5);


		calcLightProg->apply("skyBoxTexture", mSkyBox->mTexture.getTexID());
		calcLightProg->apply("P", mViewport.getPerspMat());
		calcLightProg->apply("V", mViewport.getViewMat());

		mSkyBox->Render(mViewport);

		glDepthMask(GL_TRUE);
	}

	void AlphaPassGeom() {
		const ShaderProgram * alphaPassProg = mShaderManager.getShader("alphaPassProg");
		alphaPassProg->UseProgram();

		alphaPassProg->apply("cameraPosition", cam.position);
		testScene.applyLights(alphaPassProg);
		alphaPassProg->applyTexture(10, gShadowBuffer.getDepthTexture(), false);
		alphaPassProg->apply("VPShadowMap", vpShadowMatrix());
		testScene.drawTransparentObjects(alphaPassProg, mViewport.getPerspMat(), mViewport.getViewMat());

		
		//const ShaderProgram * alphaParticleProg = mShaderManager.getShader("alpaParticleProg");
		//particle->RenderMulti(mViewport, alphaParticleProg);
		
		
	}

	void fastStachosticShadowMerge() {
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		auto mergeShader = mShaderManager.getShader("shadowStachosticMergeFast");
		mergeShader->UseProgram();
		glDepthMask(false);

		//#####################
		//PHASE 0
		glViewport(0, 0, 1280, 720);
		gSingleTexBuffer.bindForWriting();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gStachosticBuffer.getTexture());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture(GBuffer::GBUFFER_TEX_TYPE_POSITION));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getDepthTexture());

		mergeShader->apply("phase", 0);
		mergeShader->apply("VPShadowMap", vpShadowMatrix());

		mScreenQuad->RenderVertices();

		//###################
		//PHASE 1
		gSingleTexBuffer.bindForWriting();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gSingleTexBuffer.getTexture());

		mergeShader->apply("phase", 1);

		mScreenQuad->RenderVertices();


		//###################
		//PHASE 2
		gSingleTexBuffer.bindForWriting();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gSingleTexBuffer.getTexture());

		mergeShader->apply("phase", 2);

		mScreenQuad->RenderVertices();
	}

	void applyTransparentObjectToShader(const TransparentModelObject& object, const ShaderProgram* shader) {
		shader->apply("alpha", object.getAlpha());
		shader->apply("eta", object.getEta());
		shader->apply("transmitive", object.getTransmitive());
	}

	Matrix4x4 vpShadowMatrix() {
		//TODO Switched off
		const Vec3 invLight = Vector3f(1, 0, 0);// Vec3 invLight = mLightManager->getDirectionLightList()[0].GetDirection()*(-1);

		const Matrix4x4 perspMat = Matrix4x4::ortographic(Vector3f(-50, -20, 50), Vector3f(50, 50, -50));
		const Matrix4x4 cameraMat = Matrix4x4::lookat(invLight, Vector3f(0, 0, 0));
		Matrix4x4 vpMat = cameraMat*perspMat;

		float v[16] = { 0.5f, 0, 0, 0,
			0, 0.5f, 0, 0,
			0, 0, 0.5f, 0,
			0.5f, 0.5f, 0.5f, 1.0f };
		return vpMat*Matrix4x4(v);
	}

	void ShadowMapPass() {
		//TODO Switched off
		const Vec3 invLight = Vector3f(1,0,0);// const Vec3 invLight = mLightManager->getDirectionLightList()[0].GetDirection()*(-1);		
		
		const Matrix4x4 perspMat = Matrix4x4::ortographic(Vector3f(-50, -20, 50), Vector3f(50, 50, -50));
		const Matrix4x4 cameraMat = Matrix4x4::lookat(invLight, Vector3f(0, 0, 0));
	
		const ShaderProgram * shader = mShaderManager.getShader("shadowPassProg");
		shader->UseProgram();
		testScene.drawIntransparentObjects(shader, perspMat, cameraMat);
	}

	/*
	void Logic()
	{
		Matrix4x4 view_mat = Matrix4x4::viewMatrix(cam.rotation, cam.position);
		//mParticles->logic(0.1f, view_mat);
		
		const float  ROTATION_SPEED = 0.005f; // Przesuniecie o jeden piksel na klatke myszka przeklada sie na ta ilosc obrotow w ciagu sekundy
		const float MOVE_SPEED = 10.0f; // Predkos w jednostkach na sekunde
		const ControllerRec& rec = ControllerRec::GetSingleton();

		if (rec.IsDown(GLFW_KEY_Q)) {
			printf("[%f, %f, %f],[%f, %f, %f]\n", cam.position.v[0], cam.position.v[1], cam.position.v[2], cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2]);
		}
	
		if (rec.IsPushed(GLFW_KEY_B)) 
		{
			mViewBuffer = (BUFFERENUM)((int)mViewBuffer + 1);
			mViewBuffer = mViewBuffer == BUFFERENUM::END ? (BUFFERENUM)0 : mViewBuffer;
		}
		if (rec.IsPushed(GLFW_KEY_F)) {
			frameIndex = frameIndex >= 3 ? 0 : frameIndex + 1;
		}

		if (rec.IsDown(GLFW_KEY_S))
		{
			float v[] = { 0, 0, -MOVE_SPEED, 0 };
			Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
			Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2])*(float)mFrameElaps;
			cam.position = cam.position + move3;

		}
		if (rec.IsDown(GLFW_KEY_W))
		{
			float v[] = { 0, 0, MOVE_SPEED, 0 };
			Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
			Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2])*(float)mFrameElaps;
			cam.position = cam.position + move3;
		}
		if (rec.IsDown(GLFW_KEY_A))
		{
			float v[] = { MOVE_SPEED, 0, 0, 0 };
			Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
			Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2])*(float)mFrameElaps;
			cam.position = cam.position + move3;

		}
		if (rec.IsDown(GLFW_KEY_D))
		{
			float v[] = { -MOVE_SPEED,0, 0, 0 };
			Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
			Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2])*(float)mFrameElaps;
			cam.position = cam.position + move3;
		}

		if (rec.IsPushed(GLFW_KEY_C)) {
			NextCameraPos();
		}
		
		if (cameraMovement) {
			cam.rotation.v[1] += -(float)rec.GetMoveX() * 2.0f * PI * (float)(mFrameElaps)*ROTATION_SPEED;
			cam.rotation.v[0] += (float)rec.GetMoveY() * 2.0f * PI * (float)(mFrameElaps)*ROTATION_SPEED;
		}

		cameraMovement = rec.IsDown(GLFW_KEY_LEFT_ALT);
		
		mViewport.updateCamera(cam);

		if (ControllerRec::GetSingleton().IsDown(GLFW_KEY_P))
		{
			window.screenshot("testresult/" + GetName() + "screen.png");
		}

	}*/

public:

	WindowGL * GetWindow() {
		return &window;
	}

	void RenderFrame() {
		RenderDeffered();
	}

	int Run();
	
	std::string GetName() {
		return "Phenomonological";
	}

	void Render() {
		RenderDeffered();
	}

	int Init() {
		int result;
		if ((result = InitGL()) < 0)
			return result;

		InitScene();
		InitShaderAndBuffers();

		return 0;
	}

	Application() {
	}
};


int _tmain(int , _TCHAR* )
{
	ApplicationAbstract * app;
	app = new Application();

	//app = new AbufferApplication();
	//app = new NbufferApplication();
	//app = new StachosticTransparencyApplication();

	int result = app->Run();

	delete app;
	return result;
}


inline int Application::Run() {
	int result;
	if ((result = InitGL()) < 0)
		return result;

	InitScene();
	InitShaderAndBuffers();
	
	mTimeMeas.reset();

	double rotLight = 0.0;
	doTest();
	while (!window.isClose()) {
		TimeMeasure oneFrame = TimeMeasure::start();

		TimeMeasure checkFrame = TimeMeasure::start();
		RenderDeffered();
		
		window.finishFrame();
		
		avgDeff += checkFrame.reset() ;

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
		int sleepTime = (int)(1000.0f*(1 / 60.0f - oneFrame.measure()));
		if (sleepTime > 0) {
			Sleep(sleepTime);
		}
		
		rotLight += 2 * 3.14 / 120.0;
	}

	glfwTerminate();
	return 0;
}
