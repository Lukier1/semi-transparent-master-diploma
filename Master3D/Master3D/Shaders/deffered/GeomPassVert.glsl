#version 420

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;
layout (location = 3) in vec4 vtan;

uniform mat4 MMat, MVPMat, VMat;

out vec3 positionWorld, normalWorld;
out vec2 texCoord;
out mat3 tanToWorld;

void main() {
	texCoord = vt;
	positionWorld = (vec4(vp,1.0)*MMat).xyz;
	normalWorld = normalize(vn*mat3(MMat));

	vec3 vertex_bitangent = cross(vn, vtan.xyz) * vtan.w;
	tanToWorld = transpose(mat3(vtan, vertex_bitangent, vn));


	gl_Position = vec4(vp,1.0)*MVPMat;
}