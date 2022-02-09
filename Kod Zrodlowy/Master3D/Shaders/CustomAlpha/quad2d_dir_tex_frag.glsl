#version 400

uniform sampler2D diffuse_texture;
varying vec2 st;

out vec4 frag_color;

void main() {
	vec3 val = texture2D(diffuse_texture, st).xyz;
	frag_color = vec4(val, 1.0f);
}
