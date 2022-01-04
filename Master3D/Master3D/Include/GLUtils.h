#pragma once
#include "../stdafx.h"

void APIENTRY debug_gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, GLvoid* userParam);