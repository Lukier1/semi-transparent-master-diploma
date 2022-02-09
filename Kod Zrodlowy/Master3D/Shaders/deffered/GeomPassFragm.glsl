#version 420

in vec3 positionWorld;
in vec3 normalWorld;
in vec2 texCoord;
in mat3 tanToWorld;

layout (location = 0) out vec3 posOut; 
layout (location = 1) out vec3 diffuseOut; 
layout (location = 2) out vec3 normOut; 
layout (location = 3) out vec3 texCoordOut;

uniform sampler2D diffuse_texture;
uniform sampler2D normals_texture;

uniform mat4 MMat;
void main() {
	vec4 texOut = texture2D(diffuse_texture, texCoord);
	if (texOut.w < 0.01) {
		discard;
	} 

	vec3 normal_tan = texture(normals_texture, texCoord).xyz;
	normal_tan = normalize (normal_tan * 2.0 - 1.0);
	//works in wrong way
	normOut =normalWorld;// normal_tan*tanToWorld*mat3(MMat);
	posOut = positionWorld;
	diffuseOut = texOut.xyz;
	texCoordOut = vec3(texCoord,0.0);
}
