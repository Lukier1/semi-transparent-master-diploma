#version 400

uniform sampler2D diffuse_texture;
varying vec2 st;

out vec4 frag_color;

void main() {
	float depthVal = texture2D(diffuse_texture, st).x;
	frag_color = vec4(depthVal, depthVal, depthVal, 1.0f);
}
