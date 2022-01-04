#version 430

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;
layout (location = 3) in vec3 aOffset;

uniform mat4 MMat, MVMat, MVPMat;
uniform mat4 PMat;

out vec2 texCoord;
out vec3 positionWorld, positionCamera, normalWorld, normalCamera, positionOut;

void main() {	
	vec3 posOffset = vp;
	positionWorld = (vec4(posOffset,1.0)*MMat).xyz;
	normalWorld = normalize((vn*mat3(MMat)));
	positionCamera = (vec4(posOffset,1.0)*MVMat).xyz;
	normalCamera = normalize(vn*mat3(MVMat)).xyz;
	texCoord = vt;
	gl_Position = vec4(posOffset,1.0)*MVPMat;
}