#pragma once

#include "CobMath.h"

class ControllerRec {
public:
	void CleanStates(GLFWwindow * window, Vec2 center);

	void CalculateMove(GLFWwindow * window, Vec2 center);

	bool IsPushed(unsigned int key) const;
	bool IsReleased(unsigned int key) const;
	bool IsDown(unsigned int key) const;
	double GetMoveX() const;
	double GetMoveY() const;
	static ControllerRec& GetSingleton();
	
	static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow * window, int button, int action, int mode);
	static void KeyCallback(GLFWwindow* window, int key, int 
		
		code, int action, int mode);
private:

	ControllerRec();
	const unsigned int KEY_TABLE_SIZE = 1000;

	double centerX, centerY;
	double moveX, moveY;
	bool * pressKey, *releasedKey, *downKey;
	ControllerRec(ControllerRec &c) = delete;
	~ControllerRec();
};