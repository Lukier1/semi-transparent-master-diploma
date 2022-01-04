#pragma once

#include "../stdafx.h"

class WindowGL {
private:
	const unsigned short SIZE_W = 1280, SIZE_H = 720;
	
	double mPreviousSeconds;
	int mFrameCount;
	double sumRenderTime = 0;
	double countOfProbes = 0;

	GLFWwindow * mWindow;
	void update_fps_counter();
	static void glfw_error_callback(int error, const char* description);

public:
	WindowGL();
	
	int initWindow();
	GLFWwindow * getWindow();

	bool isClose();
	void startFrame();
	void finishFrame();
	
	void screenshot(std::string filename);

	unsigned short getScreenWidth() const;
	unsigned short getScreenHeigth() const;
};