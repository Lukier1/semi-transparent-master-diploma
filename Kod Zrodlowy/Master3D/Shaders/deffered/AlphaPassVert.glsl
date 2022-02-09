#version 420

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;
layout (location = 3) in vec3 vtan;

uniform mat4 MMat, MVMat, MVPMat;
uniform mat4 PMat;

out vec3 positionWorld, positionCamera, normalWorld, normalCamera, positionOut;

out vec3 tangentCamera, bitangentCamera;

out vec2 texCoord;

void main() {
	texCoord = vt;
	positionWorld = (vec4(vp,1.0)*MMat).xyz;
	normalWorld = normalize((vn*mat3(MMat)));
	positionCamera = (vec4(vp,1.0)*MVMat).xyz;
	normalCamera = normalize(vn*mat3(MVMat)).xyz;

	gl_Position = vec4(positionCamera,1.0)*PMat;
	positionOut = gl_Position.xyz;
}