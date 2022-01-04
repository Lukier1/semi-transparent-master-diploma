#version 420

layout (location = 0) out float depthOut; 

void main() {
	depthOut = gl_FragCoord.z;
}
