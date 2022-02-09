#version 400

uniform mat4 PMat;
uniform mat4 VMat;
uniform mat4 MMat;
uniform mat4 MVMat;

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt; 
layout (location = 3) in vec3 position; 

out vec3 positionCamera, normalCamera;
out vec2 texCoord;

void main () {
	//mat4 matM = mat4(1.0):
	//mat4 MVMat = VMat*MMAt;
	texCoord = vt;
	positionCamera = (vec4(vp+position,1.0)*MVMat).xyz;
	normalCamera = normalize(vn*mat3(MVMat)).xyz;
	gl_Position =  vec4(positionCamera, 1.0)*PMat;

/*
	texCoord = vt;
	positionCamera = (vec4(vp,1.0)*MVMat).xyz;
	normalCamera = normalize(vn*mat3(MVMat)).xyz;
	gl_Position =  vec4(positionCamera, 1.0)*PMat;
*/}