#version 420

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;

uniform mat4 MVPMat;
uniform mat4 MMat;
uniform mat4 MVMat;

out vec3 positionWorld;
out vec3 normalCamera;
out vec2 texCoord;

void main() {
	positionWorld = (vec4(vp,1.0)*MMat).xyz;
	normalCamera = normalize(vn*mat3(MVMat));
	gl_Position =  vec4(vp,1.0)*MVPMat;
	texCoord = vt;
}