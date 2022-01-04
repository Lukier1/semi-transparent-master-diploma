#version 420

layout (location = 0) in vec3 vp;

uniform mat4 MMat, MVPMat, VMat;


void main() {

	gl_Position = vec4(vp,1.0)*MVPMat;
}