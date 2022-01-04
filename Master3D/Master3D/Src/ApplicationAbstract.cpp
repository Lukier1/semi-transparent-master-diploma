#include "stdafx.h"

#include <ApplicationAbstract.h>
#include <ControllerRec.h>
#include <TimeMeasure.h>
void ApplicationAbstract::ApplyAlpha(const Resources::ShaderProgram* shaderProgram) {
	shaderProgram->apply("alpha", this->alpha);
}

void ApplicationAbstract::SetAlpha(float alpha) {
	this->alpha = alpha;
}

ApplicationAbstract::ApplicationAbstract() : cam(Vector3f(0, 5.0f, 10.0f), Vector3f(0.4f, -PI, 0)), mViewport(Vector2f(800, 600), cam), mTimeMeas(TimeMeasure::start()) {
	mViewport.setViewSize(Vector2f(window.getScreenWidth(), window.getScreenHeigth()));
	mViewport.setPersp(1.0f, 60.0f);
}

ApplicationAbstract::~ApplicationAbstract() {

}

void ApplicationAbstract::Logic()
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
	if (rec.IsReleased(GLFW_KEY_P)) {
		static int index = 0;
		window.screenshot("testresult/" + GetName() + "_" + std::to_string(++index) +  "_screen.png");
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

void ApplicationAbstract::NextCameraPos() {
	static int ind = -1;
	float scale = 5;
	float camPos[4][2][3] = {
		{{10.104733, 7.106497, 3.431073},{-0.001761, -1.534323, 0.000000}},
			{{7.063844, 13.699590, 1.455959},{0.654297, -1.373446, 0.000000}},
			{{8.618509, 1.351903, 3.387586},{-0.617451, -1.606352, 0.000000}}
	};



	ind = (ind + 1) % 3;
	cam.position = Vector3f(camPos[ind][0][0], camPos[ind][0][1], camPos[ind][0][2]);
	cam.rotation = Vector3f(camPos[ind][1][0], camPos[ind][1][1], camPos[ind][1][2]);

	float v[] = { 0,0,0,0 };
	v[2] = -0.0f;
	Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2]) * Vec4(v);
	Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
	cam.position = cam.position + move3;
}


int ApplicationAbstract::InitGL()
{
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);

	int maxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	std::cout << "Max samples " << maxSamples << std::endl;

	return 0;
}

WindowGL* ApplicationAbstract::GetWindow() {
	return &window;
}


void ApplicationAbstract::SetCamera(Vec3 position, Vec3 rotation) {
	cam.position = position;
	cam.rotation = rotation;
	mViewport.updateCamera(cam);
}


int ApplicationAbstract::Run()
{
	int initResult = Init();
	if (initResult < 0) {
		return initResult;
	}

	mTimeMeas.reset();

	double rotLight = 0.0;

	doTest();

	while (!window.isClose()) {
		TimeMeasure oneFrame = TimeMeasure::start();

		Render();

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
	}

	glfwTerminate();
	return 0;
}

void ApplicationAbstract::doTest() {
	TimeMeasure testTime = TimeMeasure::start();
	int alpha = 10;
//	for(int alpha = 0; alpha <= 20; ++alpha) {
		for (int i = 0; i < 3; ++i) {
			getScene()->setAlpha(alpha / 20.0f); // To test with constant alpha
			//getScene()->randAlpha(alpha); //To test with random alpha
			NextCameraPos();
			mViewport.updateCamera(cam);
			Render();
			window.finishFrame();
			std::string alphaStr = std::to_string(alpha / 20.0f);
			alphaStr = alphaStr.substr(0, alpha % 2 == 1 || alpha == 1 ? 4 : 3);
		
			std::string methodName = GetName();
			//window.screenshot("testresult/" +  std::to_string(i) + "/" + alphaStr + "/" + methodName + "_" + std::to_string(i) + "_inord_screen.png");
		}
	//}
	std::cout << "Test time: " << testTime.measure() << " s" << std::endl;
}

void ApplicationAbstract::RenderFrame() {
	Render();
}