#version 430

layout (location = 0) in vec2 vp;

out vec2 st;

void main() {
	st = (vp + 1.0) * 0.5;
	gl_Position = vec4(vp , 0.0, 1.0);
}