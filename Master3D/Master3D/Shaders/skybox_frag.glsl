#version 400

in vec3 texcoords;

uniform samplerCube cube_texture;

out vec4 frag_color;

void main() {
	//frag_color = texture (cube_texture, texcoords);
	frag_color = vec4(1,0,0,1);
}
